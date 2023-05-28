
void macro2()
{
    auto canvas = new TCanvas();
    canvas->Divide(3, 2);

    double bound = TMath::Sqrt(TMath::Pi());

    auto f = new TF2("f","[0]*sin(x*y)",0,bound,0,bound);
    float C = 1.0;
    f->SetParameter(0, C);

    canvas->cd(1);
    double integral = f->Integral(0,1.5,0,1.5);
    auto f_norm = new TF2("f","[0]*sin(x*y)",0,bound,0,bound);
    f_norm->SetParameter(0, 1./integral);
    cout << integral << endl;
    f_norm->Draw("surf1");

    canvas->cd(2);
    auto hist = new TH2D ("hist", "random numbers;x;y;z", 100, 0, 1.5, 100, 0, 1.5);
    int n_entries = 1000000;
    for (int i = 0; i < n_entries; i++)
    {
        double x_temp, y_temp;
        f_norm->GetRandom2 (x_temp, y_temp);
        double weight = 1./(double)n_entries;
        hist->Fill(x_temp, y_temp, weight);
    }
    hist->Draw("surf");

    canvas->cd(3);
    auto dist = (TH2D*)hist->Clone("Dystrybuanta");
    for (int i = 1; i<= dist->GetNbinsX(); i++)
    {   
        for (int j = 1; j <= dist->GetNbinsY(); j++)
        {
            auto temp = hist->Integral(1, i, 1, j);
            dist->SetBinContent(i, j, temp);
        }
    }
    dist->Draw("surf");

    
    canvas->cd(4);
    auto g_x = new TF1("g_x", 
        [f_norm](double* x, double*p) {
         auto dx = 0.0001;
         return (1./0.0001)*f_norm->Integral(x[0],x[0]+dx, 0, 1.5); 
         }, 
         0., 1.5, 1);
    g_x->Draw();

    canvas->cd(5);
    auto h_y = new TF1("h_y", 
        [f_norm](double* x, double*p) {
         auto dy = 0.0001;
         return (1./dy)*f_norm->Integral(0,1.5, x[0], x[0] + dy); 
         }, 
         0., 1.5, 1);
    h_y->Draw();
    
    cout<< "MeanX: " << hist->GetMean(1) << "\n";
    cout<< "MeanXErr: " << hist->GetMeanError(1) << "\n";

    cout<< "MeanY: " << hist->GetMean(1) << "\n";
    cout<< "MeanYErr: " << hist->GetMeanError(1) << "\n";

    cout<<"StdDevX: "<<hist->GetStdDev(1) << "\n";
    cout<<"StdDevY: "<<hist->GetStdDev(2) << "\n";
    cout<< "Covariance: "<< hist->GetCovariance() <<"\n";
    cout<< "Correlation: "<< hist->GetCorrelationFactor() <<"\n";

}
