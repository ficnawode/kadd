
double calculate_T(TH1F* hist, TF1* fitFunc)
{
    double N = hist->GetNbinsX();
    std::vector<double> n;
    std::vector<double> p;
    for(int i = 1; i < N+1; i++)
    {
        n.push_back(hist->GetBinContent(i));
        p.push_back(fitFunc->Eval((hist->GetBinCenter(i))));
    }
    assert(n.size() == p.size());
    double T = 0;
    for (int i = 0; i < N; i++)
    {
        double p_k = p[i], n_k = n[i];
        T += 
(n_k - p_k)*(n_k - p_k)/(p_k);
    }
    return T;
}

double validate_hypothesis(double T, double ndf, double alpha )
{
    bool validated = T < TMath::ChisquareQuantile(1 - alpha, ndf);
    cout << ((validated)? "valid": "invalid") << '\n';
    return validated;
}

void macro()
{
    std::vector<float> a;
    ifstream in;
    in.open("in/dane-zad9.txt");
    auto hist = new TH1F("a", "a", 10, 0, 10.);
    while (true) {
        float temp;
        in >> temp;
        if( in.eof() ) break;
        cout << temp << endl;
        a.push_back(temp);
        hist->Fill(temp);
    }
    hist->Draw("histe");
    auto poisson = new TF1("", "[1]*TMath::Poisson(x, [0])", 0,10);
    poisson->SetParameter(0, 10);
    poisson->SetParameter(1, hist->GetMaximum());
    
    double alpha = 0.1;
    auto fit = hist->Fit(poisson, "S");
    poisson->SetLineColor(kRed);
    poisson->DrawCopy("same");
    validate_hypothesis(calculate_T(hist, poisson)
                            , hist->GetNbinsX() - poisson->GetNpar(), alpha);
    
    cout << "\n value of T:"<< calculate_T(hist, poisson)<< '\n';
    cout << "\n value of root chi2:"<< fit->Chi2() << '\n';

    fit = hist->Fit(poisson, "L");
    poisson->SetLineColor(kBlue);
    poisson->DrawCopy("same");
    validate_hypothesis(calculate_T(hist, poisson)
                            , hist->GetNbinsX() - poisson->GetNpar(), alpha);

    fit = hist->Fit(poisson, "W");
    poisson->SetLineColor(kGreen);
    poisson->DrawCopy("same");
    validate_hypothesis(calculate_T(hist, poisson)
                            , hist->GetNbinsX() - poisson->GetNpar(), alpha);

    fit = hist->Fit(poisson, "M");
    poisson->SetLineColor(kPink);
    poisson->DrawCopy("same");
    validate_hypothesis(calculate_T(hist, poisson)
                            , hist->GetNbinsX() - poisson->GetNpar(), alpha);

    
    
}
