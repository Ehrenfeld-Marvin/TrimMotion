#include "IFstream.H"

Foam::autoPtr<Foam::OFstream> Foam::functionObjects::TrimForces::createFile
(
    const word& name,
    scalar timeValue
) const
{
	autoPtr<OFstream> osPtr;

	if (Pstream::master() && writeToFile_)
	{

		word fName(name);
		
		IFstream is(fName + ".dat");
		
		if (is.good())
		{

			osPtr.set(new OFstream(fName + ".dat", IOstreamOption(), true));
		}

		osPtr.reset(new OFstream(fName + ".dat", IOstreamOption(), true));
        
		if (!osPtr->good())
		{
			FatalIOErrorInFunction(osPtr()) << "Cannot open file"
			<< exit(FatalIOError);
		}

			initStream(osPtr());
	}

		return osPtr;
}
