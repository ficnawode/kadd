


void macro1()
{
    auto c = new TCanvas();
    c->Divide(2,1);

    auto chi2 = new TF1("chi2", 
        [](double *x, double *p){
            auto numerator = std::pow(x[0], p[0]/2 - 1)*TMath::Exp(-x[0]/2); 
            auto denominator = std::pow(2, p[0]/2)*TMath::Gamma(p[0]/2);
            return numerator/denominator;
            }, 0., 10., 1);

    auto chidist = new TF1("chidist", 
        [chi2](double *x, double *p){
            return chi2->Integral(0,x[0]);
            }, 0., 10., 1);

    for(int i = 1; i <= 20; i++)
    {
        chi2->SetParameter(0, i);
        if(i == 1)
        {
            c->cd(1);
            chi2->DrawCopy();
            c->cd(2);
            chi2->DrawIntegral();
            continue;
        }
        c->cd(1);
        chi2->DrawCopy("same");
        c->cd(2);
        chi2->DrawIntegral("same");
    }
}

