#include "macro2_data.h"

TMatrixD get_correlation_matrix(TFitResultPtr fr)
{
    auto ndf = fr->Ndf();
    auto res = TMatrixD(ndf, ndf);
    for(int i = 0 ; i < ndf; i++)
        for(int j = 0 ; i < ndf; i++)
            res(i,j) = fr->Correlation(i,j);
    return res;
}


void macro2()
{
    auto c = new TCanvas();

    unsigned int N = data::x.size();
    auto g = new TGraphErrors(N,data::x.data(),data::y.data(),{},data::uy.data());
    c->cd(1);
    auto fit = g->Fit("gaus", "S");
    
    cout<< "GAUSS:" << endl;
    cout<< "Chi2 = " << fit->Chi2() << endl;
    cout<< "Ndf = " << fit->Ndf() << endl;
    cout<< "Chi2/Ndf = " << fit->Chi2()/fit->Ndf() << endl;
    get_correlation_matrix(fit).Print();

    g->GetFunction("gaus")->SetLineColor(1);
    g->DrawClone();
    auto landau = new TF1("landau","[0]*TMath::Landau(x,[1],[2],0)", -5, 9);
    landau->SetParameters(1, 0.2,1.3);
    fit = g->Fit(landau, "S");
    
    cout<< "LANDAU:" << endl;
    cout<< "Chi2 = " << fit->Chi2() << endl;
    cout<< "Ndf = " << fit->Ndf() << endl;
    cout<< "Chi2/Ndf = " << fit->Chi2()/fit->Ndf() << endl;
    get_correlation_matrix(fit).Print();
    g->GetFunction("landau")->SetLineColor(3);


   auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetHeader("Dit","C"); // option "C" allows to center the header
   legend->AddEntry(g,"data","lep");
   legend->AddEntry(g->GetFunction("gaus"),"fit gaus","l");
   legend->AddEntry(g->GetFunction("landau"),"fit landau","l");
   legend->Draw();

    g->Draw("same");



}
