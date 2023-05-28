



double GetNumberFromFold(unsigned int N)
{
    static auto r = new TRandom3();
    double res = 0;
    for (int i = 0; i < N; i++)
        res+= r->Uniform();
    return res;
}

void macro2()
{
    auto hist = new TH1D("hist","x",400, 0.,20.);
    for(int i = 0; i < 100000; i++)
        hist->Fill(GetNumberFromFold(30));
    hist->Draw();
    auto fit = hist->Fit("gaus", "S");
    auto chi2 = fit->Chi2();
    auto ndf = fit->Ndf();
    cout << "chi2: " << chi2 << '\n';
    cout << "NDF: " << ndf << '\n';
    cout << "chi2/NDF: " << chi2/ndf << '\n';
}

