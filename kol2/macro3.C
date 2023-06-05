#include "macro2_data.h"

double PValueToSignificance(Double_t pvalue){
   return ::ROOT::Math::normal_quantile_c(pvalue,1);
}

void macro3()
{
    auto c = new TCanvas();

    unsigned int N = data::x.size();
    auto g = new TGraphErrors(N,data::x.data(),data::y.data(),{},data::uy.data());
    c->cd(1);
   auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetHeader("Fit","C"); // option "C" allows to center the header
   legend->AddEntry(g,"data","lep");

    auto fit = g->Fit("pol4", "S");
    cout<< "pol4:" << endl;
    cout<< "Chi2 = " << fit->Chi2() << endl;
    cout<< "Ndf = " << fit->Ndf() << endl;
    cout<< "Chi2/Ndf = " << fit->Chi2()/fit->Ndf() << endl;
    g->GetFunction("pol4")->SetLineColor(2);
   legend->AddEntry(g->GetFunction("pol4"),"fit pol4","l");
    g->DrawClone();

    fit = g->Fit("pol5", "S");
    cout<< "pol5:" << endl;
    cout<< "Chi2 = " << fit->Chi2() << endl;
    cout<< "Ndf = " << fit->Ndf() << endl;
    cout<< "Chi2/Ndf = " << fit->Chi2()/fit->Ndf() << endl;
    g->GetFunction("pol5")->SetLineColor(3);
   legend->AddEntry(g->GetFunction("pol5"),"fit pol5","l");
    g->DrawClone("same");

    fit = g->Fit("pol6", "S");
    cout<< "pol6:" << endl;
    cout<< "Chi2 = " << fit->Chi2() << endl;
    cout<< "Ndf = " << fit->Ndf() << endl;
    cout<< "Chi2/Ndf = " << fit->Chi2()/fit->Ndf() << endl;
    g->GetFunction("pol6")->SetLineColor(6);
    legend->AddEntry(g->GetFunction("pol6"),"fit pol6","l");
    g->DrawClone("same");

   legend->Draw();

    g->Draw("same");



}

