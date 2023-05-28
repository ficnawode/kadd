

double CalculateMonteCarlo(TF1* f, double minX, double maxX, unsigned int NPoints)
{
    double minY = f->GetMinimum(minX, maxX);
    double maxY = f->GetMaximum(minX, maxX);

    unsigned int pointsUnderCurve = 0;
    auto gen = new TRandom2(0);
    for(unsigned int i = 0; i < NPoints; i++)
    {
        auto randX = gen->Uniform(minX, maxX);
        auto randY = gen->Uniform(minY, maxY);
        pointsUnderCurve += (randY < f->Eval(randX)) ? 1 : 0;
    }
    double totalArea = (maxX - minX)*(maxY - minY);
    return ((double)pointsUnderCurve) / ((double) NPoints) * totalArea;
}

double CalculateMonteCarlo2(TF1* f, double minX, double maxX, unsigned int NPoints, TCanvas* c)
{
    double minY = f->GetMinimum(minX, maxX);
    double maxY = f->GetMaximum(minX, maxX);
    auto a = (minY - maxY)/(maxX - minX);
    auto b = maxY - (minY - maxY)/(maxX - minX) * minX + 0.001;


    auto g = new TF1("g", "[0] * x + [1]", 1,3);

    g->SetParameter(0,a);
    g->SetParameter(1,b);
    cout << "a = " << a <<'\n';
    cout << "b = " << b <<'\n';

    c->cd(1);
    g->Draw("same");

    unsigned int pointsUnderCurve = 0;
    auto gen = new TRandom2(0);
    for(unsigned int i = 0; i < NPoints; i++)
    {
        auto randX = gen->Uniform(minX, maxX);
        auto randY = gen->Uniform(0, g->Eval(randX));
        pointsUnderCurve += (randY < f->Eval(randX)) ? 1 : 0;
    }
    double totalArea = g->Integral(1,3);
    cout << "totalArea = " << totalArea << '\n';
    return ((double)pointsUnderCurve) / ((double) NPoints) * totalArea;
}

void macro()
{
    //f(x) = exp(-(3x+1/x2))sin2(x2) dla 1< x < 3
    auto c = new TCanvas();
    c->Divide(3,3);

    c->cd(1);
    auto f = new TF1("f", "exp(-(3*x + 1/(x*x)))*sin(x*x)*sin(x*x)",1,3);
    f->Draw();

    auto referenceIntegral = f->Integral(1,3);
    cout << "\\int_1^3{f} = " << referenceIntegral << '\n';
    auto vonNeumannIntegral = CalculateMonteCarlo(f,1,3, 10000);
    cout << "neumann \\int_1^3{f} = " << vonNeumannIntegral << '\n';
    cout << "relative uncertainty = " << TMath::Abs(vonNeumannIntegral - referenceIntegral) << '\n';
    
    std::vector<double> amountOfPoints = {10,100,1000,10000, 100000};
    std::vector<double> uncertainties;
    for(auto p : amountOfPoints)
    {
        auto diffh = new TH1D("h", "",100, 0,3);
        for(int i = 0; i < 100; i++)
        {
            diffh->Fill(referenceIntegral - CalculateMonteCarlo(f,1,3,p));
        }
        uncertainties.push_back(diffh->GetMean());
        cout << p << " : " << diffh->GetMean() << '\n';
        delete diffh;
    }
    
    c->cd(2);
    auto ugraph = new TGraph(uncertainties.size(), amountOfPoints.data(), uncertainties.data());
    ugraph->Draw("AC*");
    gPad->SetLogx();

    vonNeumannIntegral = CalculateMonteCarlo2(f,1,3, 10000, c);
    cout << "neumann2 \\int_1^3{f} = " << vonNeumannIntegral << '\n';
    cout << "relative uncertainty = " << TMath::Abs(vonNeumannIntegral - referenceIntegral) << '\n';


}

