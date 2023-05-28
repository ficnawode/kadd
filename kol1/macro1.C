
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

void macro1()
{
    auto canvas = new TCanvas();
    canvas->Divide(3, 2);
    
    auto f1D = new TF1("f","[0]*sin(x)*exp(x)",0,1);
    float C = 1.0;
    f1D->SetParameter(0, C);

    //canvas->cd(1);
    double integral = f1D->Integral(0,1);
    cout << "C = " << 1/integral << '\n';
    auto f1D_norm = new TF1("f","[0]*sin(x)*exp(x)",0,1);
    f1D_norm->SetParameter(0, 1./integral);
    //f1D_norm->Draw();

    canvas->cd(1);
    auto dist = new TF1("dist", 
        [f1D_norm](double* x, double*p) {
         return f1D_norm->Integral(0,x[0]); 
         }, 0., 1., 1);
    dist->Draw();

    canvas->cd(2);
    auto hist = new TH1D ("hist", "random numbers;x;y;z", 100, 0, 1);
    int n_entries = 10000;
    for (int i = 0; i < n_entries; i++)
    {
        double x_temp = f1D_norm->GetRandom();
        double weight = 1./(double)n_entries;
        hist->Fill(x_temp, weight);
    }
    hist->Draw();

    canvas->cd(3);
    auto disthist = (TH1D*)hist->Clone("dist_numeric");
    for (int i = 1; i<= disthist->GetNbinsX(); i++)
    {   
        auto temp = hist->Integral(1, i);
        disthist->SetBinContent(i, temp);
    }
    disthist->Draw();
    
    cout << "Variance: " << 
        hist->GetStdDev()*hist->GetStdDev() << '\n';
    cout << "Expected Value : " << 
        hist->GetMean() << '\n';

    Double_t median, quantile;
    quantile = 0.5; // 0.5 for "median"
    hist->ComputeIntegral(); // just a precaution
    hist->GetQuantiles(1, &median, &quantile);
    cout << "Median: " << median << '\n';
}
