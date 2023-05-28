
auto hist = new TH2F("","",100,-5,5,100,-5,5);
auto fit_f = new TF2("fit_f","[0]*(1 + [1]*exp(-(x*x/[2] + y*y/[3])) - [4]*sin(x))",-5,5,-5,5);

double Chi2(const double *xx )
{
  const double A_1 = xx[0];
  const double A_2 = xx[1];
  const double A_3 = xx[2];
  const double A_4 = xx[3];
  const double A_5 = xx[4];

  fit_f->SetParameters(A_1, A_2, A_3, A_4, A_5);
  double chi2 = 0;
  for (int i = 1; i < 101; i++)
      for (int j = 1; j < 101; j++)
      {
          const double hist_x = hist->GetXaxis()->GetBinCenter(i);
          const double hist_y = hist->GetYaxis()->GetBinCenter(j);
          const double hist_z = hist->GetBinContent(i,j);
          const double func_z = fit_f->Eval(hist_x, hist_y);
          chi2 += (func_z - hist_z)*(func_z - hist_z)/hist_z;
      }

  return chi2;
}
 
int NumericalMinimization(const char * minName = "Minuit2",
                          const char *algoName = "" ,
                          int randomSeed = -1)
{
   // create minimizer giving a name and a name (optionally) for the specific
   // algorithm
   // possible choices are:
   //     minName                  algoName
   // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
   //  Minuit2                     Fumili2
   //  Fumili
   //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
   //                              BFGS2, SteepestDescent
   //  GSLMultiFit
   //   GSLSimAn
   //   Genetic
   ROOT::Math::Minimizer* minimum =
      ROOT::Math::Factory::CreateMinimizer(minName, algoName);
   if (!minimum) {
      std::cerr << "Error: cannot create minimizer \"" << minName
                << "\". Maybe the required library was not built?" << std::endl;
      return 1;
   }
 
   // set tolerance , etc...
   minimum->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
   minimum->SetMaxIterations(10000);  // for GSL
   minimum->SetTolerance(0.001);
   minimum->SetPrintLevel(1);
 
   // create function wrapper for minimizer
   // a IMultiGenFunction type
   ROOT::Math::Functor f(&Chi2,5);
   double step[5] = {0.01,0.01, 0.01, 0.01, 0.01};
   // starting point
 
   double variable[5] = { 1000.,1.2, 1., 1., 1.};
   minimum->SetFunction(f);
 
   // Set the free variables to be minimized !
   minimum->SetVariable(0,"A_1",variable[0], step[0]);
   minimum->SetVariable(1,"A_2",variable[1], step[1]);
   minimum->SetVariable(2,"A_3",variable[2], step[2]);
   minimum->SetVariable(3,"A_4",variable[3], step[3]);
   minimum->SetVariable(4,"A_5",variable[4], step[4]);
 
   // do the minimization
   minimum->Minimize();
 
   const double *xs = minimum->X();
   std::cout << "Minimum: f(" << xs[0] << "," << xs[1] <<"," << xs[2] << "," << xs[3] <<"," << xs[4] << "): "
             << minimum->MinValue()  << std::endl;
 
   return 0;
}

void macro()
{
    auto c = new TCanvas();
    c->Divide(2,2);

    ifstream in("dane-zad11.txt");
    string header;

    std::getline(in, header);

    cout << header << '\n';
    for(int i = 0; i < 100*100; i++)
    {
        float x;
        float y;
        float n;
        
        in >> x ;
        in >> y ;
        in >> n ;
        cout<< x << " ";
        cout<< y << " ";
        cout<< n << "\n";
        hist->SetBinContent(hist->GetXaxis()->FindBin(x),hist->GetYaxis()->FindBin(y),n);
    }
    c->cd(1);
    hist->Draw("surf1");

    c->cd(2);
    fit_f->SetParameters(1000,1,1,1,0.3); 
    fit_f->Draw("surf1");

    hist->Fit(fit_f);

    cout << "default" << endl;
    NumericalMinimization();
    
    cout << "Minuit2 + Simplex" << endl;
    NumericalMinimization("Minuit2", "Simplex");

    cout << "GSLSimAn + Combined" << endl;
    NumericalMinimization("GSLSimAn", "Combined");

    cout << "Minuit2 + Combined" << endl;
    NumericalMinimization("Minuit2", "Combined");
}
