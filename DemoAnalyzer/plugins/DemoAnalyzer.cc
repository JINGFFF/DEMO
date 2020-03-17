// -*- C++ -*-
//
// Package:    Demo/DemoAnalyzer
// Class:      DemoAnalyzer
// 
/**\class DemoAnalyzer DemoAnalyzer.cc Demo/DemoAnalyzer/plugins/DemoAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jing Peng
//         Created:  Tue, 09 Apr 2019 09:36:55 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include "TTree.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class DemoAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit DemoAnalyzer(const edm::ParameterSet&);
      ~DemoAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
       edm::Service<TFileService> fs;
       edm::EDGetTokenT<GenEventInfoProduct> GenToken_;
       edm::EDGetTokenT<LHEEventProduct> LheToken_;

       double theWeight, zpt, px1, px2, py1, py2;
       int nisr;
       double pweight[10];
       TTree* outTree_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DemoAnalyzer::DemoAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   //usesResource("TFileService");
   GenToken_=consumes<GenEventInfoProduct> (iConfig.getParameter<edm::InputTag>( "gentag") ) ;
   LheToken_=consumes<LHEEventProduct> (iConfig.getParameter<edm::InputTag>( "lhetag"));
   outTree_ = fs->make<TTree>("ntree","ntree");
   //outTree_->Branch("nisr" ,&nisr ,"nisr/I");
   outTree_->Branch("zpt" ,&zpt ,"zpt/D");
   outTree_->Branch("theWeight" ,&theWeight ,"theWeight/D");
   outTree_->Branch("px1" ,&px1 ,"px1/D");
   outTree_->Branch("px2" ,&px2 ,"px2/D");
   outTree_->Branch("py1" ,&py1 ,"py1/D");
   outTree_->Branch("py2" ,&py2 ,"py2/D");

}


DemoAnalyzer::~DemoAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DemoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   edm::Handle<GenEventInfoProduct> genEvtInfo;
   iEvent.getByToken(GenToken_,genEvtInfo);
   theWeight = genEvtInfo->weight();

   edm::Handle<LHEEventProduct> wgtsource;
   iEvent.getByToken(LheToken_, wgtsource);
     for(size_t ki=0; ki<10; ki++) {
          pweight[ki]=wgtsource->weights()[ki].wgt/wgtsource->originalXWGTUP();
                            //std::cout<<i<<" "<<pweight[ki]<<std::endl;
     }
     nisr=0; zpt=0.;
     for ( unsigned int icount = 0 ; icount < (unsigned int) wgtsource->hepeup().NUP; icount++ ) {
         unsigned int pdgid = abs(wgtsource->hepeup().IDUP[icount]);
         int status = wgtsource->hepeup().ISTUP[icount];
         //int mom1_idx = wgtsource->hepeup().MOTHUP[icount].first;
         //int mom2_idx = wgtsource->hepeup().MOTHUP[icount].second;
         //int mom1id = mom1_idx==0 ? 0 : abs(wgtsource->hepeup().IDUP[mom1_idx-1]);
         //int mom2id = mom2_idx==0 ? 0 : abs(wgtsource->hepeup().IDUP[mom2_idx-1]);
         float px = (wgtsource->hepeup().PUP[icount])[0];
         float py = (wgtsource->hepeup().PUP[icount])[1];
         //float pt = sqrt(px*px+py*py);
         
         //if(pdgid==23) {

           //    zpt= pt;
            //   outTree_->Fill();
         // }
      if(status==1 && abs(pdgid)>=11 &&abs(pdgid)<=15) {
		nisr++;
      
      		if(nisr==1) {px1 = px; py1 = py;}
      		if(nisr==2) {px2 = px; py2 = py;}
      }
     } // Loop over generator particles
     zpt = sqrt((px1 + px2)*(px1 + px2)+(py1 + py2)*(py1 + py2));
     if(nisr==2) outTree_->Fill();
      //


}

// ------------ method called once each job just before starting event loop  ------------
void 
DemoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DemoAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DemoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DemoAnalyzer);
