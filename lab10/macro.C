
double fit(int deg, int N, double *tj, double *yj, double* sigmaj, double *pars, double *parssigma)
{
    TMatrixD A(N,deg);
    TMatrixD H(N,N);
    TMatrixD G(N,N);
    TMatrixD y(N, 1);

    for(int i = 0; i < N; i++)
    {
        H(i,i) = 1/TMath::Power(sigmaj[i], 2);
        G(i,i) = 1/sigmaj[i];
        y(i, 0) = yj[i];
        for(int j = 0; j < deg; j++)
        {
            A(i,j) = TMath::Power(tj[i], j);
        }
    }

    cout<< "A:" << endl;
    A.Print();
    cout<< "H:" << endl;
    H.Print();
    cout<< "G:" << endl;
    G.Print();
    cout<< "y:" << endl;
    y.Print();
    
    TMatrixD A_prime = H * A;
    TMatrixD c_prime = H * y;

    cout<< "A':" << endl;
    A_prime.Print();
    cout<< "c':" << endl;
    c_prime.Print();

    TMatrixD A_prime_T = TMatrixD(A_prime).T();
    cout<< "A':" << endl;
    A_prime.Print();
    cout<< "A'T:" << endl;
    A_prime_T.Print();
    
    TMatrixD cov = (A_prime_T * A_prime).Invert();
    cout<< "cov:" << endl;
    cov.Print();
    TMatrixD x_tilda = cov*(A_prime_T * c_prime);
    //return via pars
    for (int i = 0 ; i < deg; i++)
    {
        pars[i] = x_tilda(i,0);
        parssigma[i] = cov(i,i);
    }

    cout<< "x~:" << endl;
    x_tilda.Print();

    TMatrixD eta_tilda = -1.*(A*x_tilda);
    

    TMatrixD y_minus_eta_tilda_T = TMatrixD(y - eta_tilda).T();

    auto M = y_minus_eta_tilda_T * G * (y - eta_tilda); 
    M.Print();

    return M(0,0);
}

double calculate_y(double x, int deg, double *pars)
{
    double y = 0;
    for (int i = 0; i < deg; i++)
    {
        y += pars[i]*TMath::Power(x,i);
    }
    return y;
}

void macro()
{
    double x[] = { -0.9, -0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9};
    double y[] = { 86, 50, 26, 24, 20, 55, 113, 186, 339, 601};
    double ux[] = {0,0,0,0,0,0,0,0,0,0,0};
    double uy[] = {10, 14, 13, 9, 13, 12, 12, 13, 11, 14};
    const int N = 10;
    double pars[] = {0,0,0,0,0,0};
    double parssigma[] = {0,0,0,0,0,0};
    

    auto c = new TCanvas();
    c->Divide(1,1);

    auto b = fit(6, N, x, y, uy, pars, parssigma);

    std::vector<double> fit_x;
    std::vector<double> fit_y;
    for (double i = -1; i < 1; i+=0.001)
    {
        cout << i << endl;
        fit_x.push_back(i);
        fit_y.push_back(calculate_y(i, 6, pars));
        cout << calculate_y(i, 6, pars)<< endl;
    }

    c->cd(1);
    auto gr = new TGraphErrors(N, x,y,ux,uy);
    gr->Draw();
    
    auto gr1 = new TGraph(fit_x.size(), fit_x.data(),fit_y.data());
    gr1->Draw("same");
}
