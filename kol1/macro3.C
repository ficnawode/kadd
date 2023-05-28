
double calculate_integral_error(double A, double A_prime, 
                                        unsigned int n)
{
    auto p = A/A_prime;
    return A_prime/TMath::Sqrt(n) * TMath::Sqrt(p*(1-p));
}

std::array<double, 2> calculate_monte_carlo(TF1* f, double minX, 
            double minY, double maxX, double maxY, 
                                unsigned int NPoints)
{
    std::array<double, 2> res = {0,0};
    unsigned int pointsUnderCurve = 0;
    auto gen = new TRandom2(0);
    for(unsigned int i = 0; i < NPoints; i++)
    {
        auto randX = gen->Uniform(minX, maxX);
        auto randY = gen->Uniform(minY, maxY);
        pointsUnderCurve += (randY < f->Eval(randX)) ? 1 : 0;
    }
    double totalArea = (maxX - minX)*(maxY - minY);
    res[0] = ((double)pointsUnderCurve) / ((double) NPoints) * totalArea;
    res[1] = calculate_integral_error(res[0], totalArea, pointsUnderCurve);
    return res;
}


void macro3()
{
    auto canvas = new TCanvas();
    canvas->Divide(2, 2);

    canvas->cd(1);
    auto A = new TF1("A", "1/x * sin(x)", 0, TMath::Pi()); 
    A->Draw();

    auto integral = calculate_monte_carlo(A, 0, 0, TMath::Pi(), 1, 100000);
    cout << "integral value : " << integral[0] << '\n';
    cout << "integral error : " << integral[1] << '\n';
    auto trueintval = A->Integral (0,TMath::Pi());
    cout << "true integral value : " << trueintval << '\n';
    cout << "true - montecarlo integral value : " << 
        TMath::Abs(trueintval - integral[0]) << '\n';
    cout << "is the difference smaller than the integral error? : " << 
        ((TMath::Abs(trueintval - integral[0]) <= integral[1])? "Yes" : "No") << '\n';
}
