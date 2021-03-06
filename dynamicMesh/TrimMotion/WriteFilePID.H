void Foam::solidBodyMotionFunctions::TrimMotion::writePIDdata
(
	string FileName,
	double Target,			
	float K_P,			
	float K_I,
	float K_D,
	double IST,
	double e_t_SUMME,
	double e_t,
	double y_t,
	float P,
	float I,
	float D,
	double Force_Average_X,
	double Force_Average_Y,
	double Force_Average_Z,
	float time
) const
{
		string Path=time_.globalPath();
		Path=Path.append(FileName);
		Path=Path.append(PatchName);
		std::ofstream PID_Data;
		PID_Data.open(Path, std::ios_base::app);
		
		PID_Data	<< "Target: " << Target << tab			
				<< "K_P: " << K_P << tab			
				<< "K_I: " << K_I << tab
				<< "K_D: " << K_D << tab
				<< "IST: " << IST << tab
				<< "e_t_SUMME: " << e_t_SUMME << tab
				<< "e_t: " << e_t << tab
				<< "y_t: " << y_t << tab
				<< "P: " << P << tab
				<< "I: " << I << tab
				<< "D: " << D << tab  
				<< "Force_Average_X: " << Force_Average_X << tab
				<< "Force_Average_Y: " << Force_Average_Y << tab
				<< "Force_Average_Z: " << Force_Average_Z << tab
				<< "time: " << time << "\n";
}
