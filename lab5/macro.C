

void macro()
{
    ifstream in;
    double x;

    in.open("in/X1.dat");
    std::vector<double> X1;
    while(!in.eof())
    {
        in >> x;
        X1.push_back(x);
    }
    in.close();
    
    in.open("in/X2.dat");
    std::vector<double> X2;
    while(!in.eof())
    {
        in >> x;
        X2.push_back(x);
    }
    in.close();

    in.open("in/X3.dat");
    std::vector<double> X3;
    while(!in.eof())
    {
        in >> x;
        X3.push_back(x);
    }
    in.close();

    vector<double> Y1;
     vector<double> Y2;
    for(int i = 0; i < X1.size(); i++)
    {
        Y1.push_back(3 + 0.5*X1[i] + 4*X2[i]);
        Y2.push_back(2*X1[i] + 5*X2[i] + X3[i]);
    }

    
    auto canvas = new TCanvas();
    canvas->Divide(3, 3);

    canvas->cd(1);
    auto hist_X1 = new TH1D ("X1", "", 100, 0, 6);
    for (auto& x : X1)
    {
        hist_X1->Fill(x);
    }
    hist_X1->Draw();

    canvas->cd(2);
    auto hist_X2 = new TH1D ("X2", "", 100, 0, 4);
    for (auto& x : X2)
    {
        hist_X2->Fill(x);
    }
    hist_X2->Draw();

    canvas->cd(3);
    auto hist_X3 = new TH1D ("X3", "", 100, 0, 20);
    for (auto& x : X3)
    {
        hist_X3->Fill(x);
    }
    hist_X3->Draw();

    canvas->cd(4);
    auto hist_X1X2 = new TH2D ("X1X2", "", 100, 0, 6, 100, 0, 3);
    for (int i =0; i < X1.size(); i++)
    {
        hist_X1X2->Fill(X1[i], X2[i]);
    }
    hist_X1X2->Draw("colz");

    canvas->cd(5);
    auto hist_X1X3 = new TH2D ("X1X3", "", 100, 0, 6, 100, 0, 20);
    for (int i =0; i < X1.size(); i++)
    {
        hist_X1X3->Fill(X1[i], X3[i]);
    }
    hist_X1X3->Draw("colz");

    canvas->cd(6);
    auto hist_X2X3 = new TH2D ("X2X3", "", 100, 0, 4, 100, 0, 20);
    for (int i =0; i < X2.size(); i++)
    {
        hist_X2X3->Fill(X2[i], X3[i]);
    }
    hist_X2X3->Draw("colz");

    canvas->cd(7);
    auto hist_X1X2X3 = new TH3D("X1X2X3", "", 100, 0, 6, 100, 0, 4, 100, 0, 20);
    for (int i =0; i < X2.size(); i++)
    {
        hist_X1X2X3->Fill(X1[i], X2[i], X3[i]);
    }
    hist_X1X2X3->Draw("colz");

    TMatrixD cov_X1X2X3(3,3);
     cov_X1X2X3(0,0) =      hist_X1X2X3->GetCovariance(1,1) ;
     cov_X1X2X3(0,1) =      hist_X1X2X3->GetCovariance(1,2) ;
     cov_X1X2X3(0,2) =      hist_X1X2X3->GetCovariance(1,3) ;
     cov_X1X2X3(1,0) =      hist_X1X2X3->GetCovariance(2,1) ;
     cov_X1X2X3(1,1) =      hist_X1X2X3->GetCovariance(2,2) ;
     cov_X1X2X3(1,2) =      hist_X1X2X3->GetCovariance(2,3) ;
     cov_X1X2X3(2,0) =      hist_X1X2X3->GetCovariance(3,1) ;
     cov_X1X2X3(2,1) =      hist_X1X2X3->GetCovariance(3,2) ;
     cov_X1X2X3(2,2) =      hist_X1X2X3->GetCovariance(3,3) ;

    cov_X1X2X3.Print();

    TMatrixD T_Y1Y2(2,3);
    T_Y1Y2(0,0) = 0.5;
    T_Y1Y2(0,1) = 4;
    T_Y1Y2(0,2) = 0;
    T_Y1Y2(1,0) = 2;
    T_Y1Y2(1,1) = 5;
    T_Y1Y2(1,2) = 1;
    T_Y1Y2.Print();

    TMatrixD T_Y1Y2_transposed(T_Y1Y2);
    T_Y1Y2_transposed.Transpose(T_Y1Y2_transposed);
    T_Y1Y2.Print();
    T_Y1Y2_transposed.Print();
    TMatrixD cov_Y1Y2 = T_Y1Y2*cov_X1X2X3*T_Y1Y2_transposed;
    cov_Y1Y2.Print();
    
    canvas->cd(8);
    auto hist_Y1 = new TH1D ("Y1", "", 100, 0, 20);
    for (auto& x : Y1)
    {
        hist_Y1->Fill(x);
    }
    hist_Y1->Draw();

    canvas->cd(9);
    auto hist_Y2 = new TH1D ("Y2", "", 100, 0, 40);
    for (auto& x : Y2)
    {
        hist_Y2->Fill(x);
    }
    hist_Y2->Draw();

    cout << "corr(Y_1) = " << cov_Y1Y2(0,1)/(sqrt(cov_Y1Y2(0,0))*sqrt(cov_Y1Y2(1,1))) << '\n';
    cout << "corr(Y_2) = " << cov_Y1Y2(1,0)/(sqrt(cov_Y1Y2(0,0))*sqrt(cov_Y1Y2(1,1))) << '\n';

}
