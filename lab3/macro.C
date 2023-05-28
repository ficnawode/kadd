void macro()
{
    auto canvas = new TCanvas();
    canvas->Divide(2, 2);

    auto f = new TF2("f","[0]*sin(x + [1])*sin(y + [2])",0,1,0,1);
    float C = 1.0;
    float a = 1.0;
    float b = 1.0;
    f->SetParameter(0, C);
    f->SetParameter(1, a);
    f->SetParameter(2, b);

    canvas->cd(1);
    auto integral = f->Integral(0,1,0,1);
    auto f_norm = new TF2("f","[0]*sin(x + [1])*sin(y + [2])",0,1,0,1);
    f_norm->SetParameter(0, 1/integral);
    f_norm->SetParameter(1, a);
    f_norm->SetParameter(2, b);
    f_norm->Draw("surf1");

    canvas->cd(2);
    auto dist = new TF2("f2", 
        [f_norm](double* x, double*p) {
         return f_norm->Integral(0,x[0], 0,x[1]); 
    dist->Draw("surf1");

    canvas->cd(3);
    auto h_x = new TF1("h_x", 
        [f_norm](double* x, double*p) {
         auto dx = 0.0001;
         return (1./0.0001)*f_norm->Integral(x[0],x[0]+dx, 0, 1); 
         }, 
         0., 1., 1);
    h_x->Draw();

    canvas->cd(4);
    auto g_y = new TF1("g_y", 
        [f_norm](double* x, double*p) {
         auto dy = 0.0001;
         return (1./dy)*f_norm->Integral(0,1, x[0], x[0] + dy); 
         }, 
         0., 1., 1);
    g_y->Draw();
}

