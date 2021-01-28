/*Foam::scalar*/ void Foam::solidBodyMotionFunctions::TrimMotion::Control_Variables(scalar* Amplitude_Value, scalar* phi_Value) const
{
/*++++++++++++++++++++++++++++	DECLARE AND RESET VARIABLES	+++++++++++++++++++++++++++*/

	scalar time=time_.value();
	scalar delta_t=time_.deltaT().value();
	
	Time_Interval_Value = (NuOfOsc*2*3.141592653)/omega_PID;
	Ramp_Time = Time_Interval_Value/4;

	float Force_X_Value=0;
	float Force_Y_Value=0;
	float Force_Z_Value=0;
	
	int Average_Divider=0;
	
    	Force_Average_X = 0;							
	Force_Average_Z = 0;
	Force_Average_Y = 0;
	
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	if(time > Read_Time_Interval)
	{
		#include "ReadFile.H"					//read Trim_Forces.dat

      		if(Average_Divider==0) Average_Divider=1;		//if condition is somehow ignored, causes divider to be 0

      		Force_Average_X /= (Average_Divider);			//calculate Force average
		Force_Average_Y /= (Average_Divider);
     		Force_Average_Z /= (Average_Divider);
     		
     		PatchName.find("1");
     		
/*#################	AMPLITUDE - PID	##############################*/     		

		amplitude_average = sqrt(sqr(Force_Average_X)+sqr(Force_Average_Y));
		amplitude_target = Target_Force;
    		amplitude_y = PID_EQUATION
     		(
     			amplitude_target, 
     			amplitude_average, 
     			Amplitude_K_P, 
     			Amplitude_K_I, 
     			Amplitude_K_D,
			&e_t_SUMME_AMP,
     			&IST_alt_AMP,
     			&e_t_alt_AMP,
     			&P_AMP,
			&I_AMP,
			&D_AMP,
			&IST_AMP,
			&e_t_AMP
     		);							//PID-Controller
     		if(amplitude_y<=-50) amplitude_y = -50;		//Max AOA
     		
     		if(Pstream::master() && PatchName=="_Blade1")
     		{
     			writePIDdata
			(
				"/Amplitude-PID-Data",
				amplitude_target,			
     				Amplitude_K_P, 
     				Amplitude_K_I, 
     				Amplitude_K_D,
				IST_AMP,
				e_t_SUMME_AMP,
				e_t_AMP,
				amplitude_y,
				P_AMP,
				I_AMP,
				D_AMP,
				Force_Average_X,
				Force_Average_Y,
				Force_Average_Z,
				time
			);
			
			Back_up_Data_PID
			(
				"/Amplitude-BackUp-Data",
				time,
				IST_alt_AMP,
				e_t_SUMME_AMP,
				amplitude_y
			);
		}
		
/*#############################################################################*/    

/*++++++++++++++++++++++++++	PHASE - PID	+++++++++++++++++++++++++++++++++*/  	

		deg_to_rad=(angle_F_X*2*pi)/360;
     		phi_target = Target_Force*sin(deg_to_rad)*(-1);
		phi_y = PID_EQUATION
		(
			phi_target, 
			Force_Average_X, 
			Phi_K_P, 
			Phi_K_I, 
			Phi_K_D,
			&e_t_SUMME_PHI,
     			&IST_alt_PHI,
     			&e_t_alt_PHI,
     			&P_PHI,
			&I_PHI,
			&D_PHI,
			&IST_PHI,
			&e_t_PHI
		);
		
		if(Pstream::master() && PatchName=="_Blade1")
		{
		 
			writePIDdata
			(
				"/Phi-PID-Data",
				phi_target,			
     				Phi_K_P, 
     				Phi_K_I, 
     				Phi_K_D,
				IST_PHI,
				e_t_SUMME_PHI,
				e_t_PHI,
				phi_y,
				P_PHI,
				I_PHI,
				D_PHI,
				Force_Average_X,
				Force_Average_Y,
				Force_Average_Z,
				time
			);
			
			Back_up_Data_PID
			(
				"/Phi-BackUp-Data",
				time,
				IST_alt_PHI,
				e_t_SUMME_PHI,
				phi_y
			);
		}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/		

		Ramp_change=true;					//activate ramp function
		Ramp_Time_Counter=Ramp_Time;				//reset time for ramp function
		
		Read_Time_Interval += Time_Interval_Value; 		//set new time intervals
	}

	
	if(Ramp_change && Ramp_Time_Counter>0)			//ramp function to change the CVs linaer over time
	{
		delta_amplitude 	= amplitude_y 	- amplitude_ramp;
		delta_phi 		= phi_y 	- phi_ramp;
		
		amplitude_ramp 	= amplitude_0 	+ (delta_t * delta_amplitude) 	/ Ramp_Time_Counter;
		phi_ramp 		= phi_0 	+ (delta_t * delta_phi) 		/ Ramp_Time_Counter;
		
		Ramp_Time_Counter -= delta_t;
		
		amplitude_0 	= amplitude_ramp;
		phi_0 		= phi_ramp;

		if(Ramp_Time_Counter<0) 
		{
			Ramp_change = false;
			amplitude_ramp 	= amplitude_y;
			phi_ramp 		= phi_y;
		}
	}
	
	*Amplitude_Value 	= amplitude_ramp;
	*phi_Value 		= phi_ramp;
}
