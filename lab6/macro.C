
int LCG()
{
    static constexpr int a = 16807;
    static constexpr int m = 97;
    static constexpr int c =2137;
    static int x_prev = c;

    int x = (a*x_prev + c) % m;

    x_prev = x;
    return x;
}

void macro()
{
    auto canvas = new TCanvas();
    canvas->Divide(3, 4);
    
    std::vector<int> rand;
    for (int i = 0; i < 10000;i++)
        rand.push_back(LCG());

    canvas->cd(1);
    auto hist_X1 = new TH1D ("X1", "", 104, -2, 102);
   for (auto& x : rand)
        hist_X1->Fill(x);

    hist_X1->Draw();

    canvas->cd(2);
    auto spectest = new TGraph(rand.size()-1, &rand[0], &rand[1]);
    spectest->SetTitle("Graph title;X title;Y title");
    spectest->Draw("AP");


    canvas->cd(3);
    auto f_x = new TF1("f_x", 
        [](double* x, double*p) {
         auto dx = 0.0001;
         return (1./p[0])*exp(-x[0]/p[0]); 
         }, 
         0., 10., 1);
    f_x->SetParameter(0, 1);
    f_x->Draw();

    canvas->cd(4);
    auto dist = new TF1("dist", 
        [f_x](double* x, double*p) {
         return f_x->Integral(0,x[0]); 
         }, 
         0., 10., 1);
    dist->Draw();

    canvas->cd(5);
    auto invdist = new TF1("invdist", 
        [dist](double* x, double*p) {
         return dist->GetX(x[0]); 
         }, 
         0., 0.99, 1);
    invdist->Draw();
    
    canvas->cd(6);
    auto hist_X2 = new TH1D ("X3", "", 100, 0, 10);
    for (auto& x : rand)
        hist_X2->Fill(invdist->Eval(((double)x/100.)));
    hist_X2->Draw();

    auto randgen = new TRandom();
    std::vector<double> rand2;
    for(int i = 0; i < 10000; i++)
        rand2.push_back(randgen->Integer(100));

    canvas->cd(7);
    auto hist_X3 = new TH1D ("X3", "", 104, -2, 102);
    for (auto& x : rand2)
        hist_X3->Fill(x);
    hist_X3->Draw();

    canvas->cd(8);
    auto spectest2 = new TGraph(rand2.size()-1, &rand2[0], &rand2[1]);
    spectest2->SetTitle("spectest2;X title;Y title");
    spectest2->Draw("AP");

    canvas->cd(9);
    auto hist_X4 = new TH1D ("X3", "", 100, 0, 10);
    for (auto& x : rand2)
        hist_X4->Fill(invdist->Eval(((double)x/100.)));
    hist_X4->Draw();

    canvas->cd(10);
    auto hist_X5 = new TH1D ("X5", "", 100, 0, 10);
    for (auto& x : rand2)
        hist_X5->Fill(f_x->GetRandom());
    hist_X5->Draw();
}
