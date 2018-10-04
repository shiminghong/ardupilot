#include "Copter.h"


// Run landing gear controller at 10Hz
void Copter::landinggear_update()
{
    // exit immediately if no landing gear output has been enabled
    if (!SRV_Channels::function_assigned(SRV_Channel::k_landing_gear_control)) {
        return;
    }

    // last status (deployed or retracted) used to check for changes, initialised to startup state of landing gear
    static bool last_deploy_status = landinggear.deployed();

	//useradd
	int autosafe = 0;	//safe when landing
	//useradd

    // if we are doing an automatic landing procedure, force the landing gear to deploy.
    // To-Do: should we pause the auto-land procedure to give time for gear to come down?
    if (flightmode->landing_gear_should_be_deployed()) {
        landinggear.set_position(AP_LandingGear::LandingGear_Deploy);
		//useradd
		autosafe = 1;	//safe when landing
		//useradd
    }
	//useradd
	double landingaltitude;
	landingaltitude=barometer.get_altitude();	//landingaltitude get from barometer Maybe need to change
	double landingtarget=(double)landinggear.autoretract();  //landingtarget get from AUTORTRACT
	//  landingtarget  enable or disable
	if (landingtarget != 0){
		if (landingaltitude <= (landingtarget-0.5)) {
			landinggear.set_position(AP_LandingGear::LandingGear_Deploy);  // Deploy landindgear
		}
		if ((landingaltitude > landingtarget)&&(autosafe == 0)) {
		    landinggear.set_position(AP_LandingGear::LandingGear_Retract);  // Retract landindgear
			//hal.console->printf("RRRRRR");
		}
	}
	//hal.console->printf("test:%4.4f,,,aaa:%4.4f,,,autosafe:%d,,,",(double)(landingtarget-0.5),(double)landingaltitude,(int)autosafe);
	//useradd

    // send event message to datalog if status has changed
    if (landinggear.deployed() != last_deploy_status) {
        if (landinggear.deployed()) {
            Log_Write_Event(DATA_LANDING_GEAR_DEPLOYED);
        } else {
            Log_Write_Event(DATA_LANDING_GEAR_RETRACTED);
        }
    }

    last_deploy_status = landinggear.deployed();
}
