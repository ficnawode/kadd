
double gen_plexus()
{
    static auto rand = new TRandom3();
    double res = 0;
    for(int i = 0; i < 5; i++)
    {
        res += rand->Gaus(0,1);
    }
    return res;
}

void macro1()
{
    auto hist = new TH1D("plexus", "", 100, -10, 10);
    for(int i = 0 ; i < 10000; i ++)
    {
        hist->Fill(gen_plexus());
    }
    hist->Draw();
    auto fit = hist->Fit("gaus", "S");
    
    cout<< "Chi2 = " << fit->Chi2() << endl;
    cout<< "Ndf = " << fit->Ndf() << endl;
    cout<< "Chi2/Ndf = " << fit->Chi2()/fit->Ndf() << endl;
}
