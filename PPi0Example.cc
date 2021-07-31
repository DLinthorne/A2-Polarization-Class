#include "PPi0Example.h"

PPi0Example::PPi0Example()
{ 
    
    time 	= new GH1("time", "time", 1400, -700, 700);
    time_cut 	= new GH1("time_cut", "time_cut", 1400, -700, 700);

    time_2g = new GH1("time_2g", "time_2g", 1400, -700, 700);
    time_2g_cut = new GH1("time_2g_cut","time_2g_cut", 1400, -700, 700);

    IM 	= new GH1("IM", "IM", 400, 0, 400);
    IM_2g = new GH1("IM_2g", "IM_2g", 400, 0, 400);
  
    MM	= new GH1("MM", "MM", 400, 800, 1200);     
    MM_2g	= new GH1("MM_2g", "MM_2g", 400, 800, 1200);
        
    Energy = new TH1D("Energy", "Photon_Energy", 350, 100, 450);

    Theta_1 = new TH1D("Theta_1", "Theta_1", 7, 0, 180);
    Theta_0 = new TH1D("Theta_0", "Theta_0", 7, 0, 180);

    Eng_1  = new TH1D("Energy_1", "Energy_1", 50, 100, 450);
    Eng_0  = new TH1D("Energy_0", "Energy_0", 50, 100, 450);


    Theta_1_prompt  = new TH1D("Theta_1_prompt", "Theta_1_prompt", 7, 0, 180);
    Theta_1_random  = new TH1D("Theta_1_random", "Theta_1_random", 7, 0, 180);
    Theta_0_prompt  = new TH1D("Theta_0_prompt", "Theta_0_prompt", 7, 0, 180);
    Theta_0_random  = new TH1D("Theta_0_random", "Theta_0_random", 7, 0, 180);

    TaggerAccScal = new TH1D("TaggerAccScal","TaggerAccScal",352,0,352);
}

PPi0Example::~PPi0Example()
{
}

Bool_t	PPi0Example::Init()
{
  cout << "Initialising physics analysis..." << endl;
  cout << "--------------------------------------------------" << endl << endl;

  if(!InitBackgroundCuts()) return kFALSE;
  if(!InitTargetMass()) return kFALSE;
  if(!InitTaggerChannelCuts()) return kFALSE;
  if(!InitTaggerScalers()) return kFALSE;
  cout << "--------------------------------------------------" << endl;
  return kTRUE;
}

Bool_t	PPi0Example::Start()
{
    if(!IsGoATFile())
    {
      cout << "ERROR: Input File is not a GoAT file." << endl;
      return kFALSE;
    }
    SetAsPhysicsFile();

    TraverseValidEvents();

    return kTRUE;
}

void	PPi0Example::ProcessEvent()
{
  // fill time diff (tagger - pi0), all pi0
  FillTime(*GetNeutralPions(),time);
  FillTimeCut(*GetNeutralPions(),time_cut);
  
  // fill missing mass, all pi0
  FillMissingMass(*GetNeutralPions(),MM);
  
  // fill invariant mass, all pi0
  FillMass(*GetNeutralPions(),IM);
    
    // Some neutral decays
    for (Int_t i = 0; i < GetNeutralPions()->GetNParticles(); i++)
    {
        // Fill MM for 2 photon decay
        if ((GetNeutralPions()->GetNSubParticles(i) == 2) & (GetNeutralPions()->GetNSubPhotons(i) == 2))
        {
    		// fill time diff (tagger - pi0), this pi0
          FillTime(*GetNeutralPions(),i,time_2g);
          FillTimeCut(*GetNeutralPions(),i,time_2g_cut);
    		  // fill missing mass, this pi0
          FillMissingMass(*GetNeutralPions(),i,MM_2g);        
    		  // fill invariant mass, this pi0
          FillMass(*GetNeutralPions(),i,IM_2g);
 				   // This is the meat a gravies, here's where we fill the theta histograms for a given 
           // energy cut and for different helicities.
            if (GetTrigger() -> GetHelicity() == kTRUE)
            {
              // This function can be found in PPhysics, you can change the 
              // energy there.
              FillTheta(*GetNeutralPions(),i, Theta_1_prompt, Theta_1_random);                
            }
              // do it all again for the other helicity.
            else if(GetTrigger() -> GetHelicity() == kFALSE)
            {
              FillTheta(*GetNeutralPions(),i, Theta_0_prompt, Theta_0_random);                                     
            }            
        }
    }
}

void	PPi0Example::ProcessScalerRead()
{
  // Fill Tagger Scalers
  FillScalers(GetTC_scaler_min(),GetTC_scaler_max(),TaggerAccScal);
}

Bool_t	PPi0Example::Write()
{
  // Write all GH1's and TObjects defined in this class
  return GTreeManager::Write();
}
