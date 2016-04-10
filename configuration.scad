
motor_end_height=44;
idler_end_height=28;
carriage_height=24;

platform_thickness=8;

sanguinololu=false;					// places mounting holes for the Sanguinololu controller

carbon_rods=true;					// turns on option for carbon diagonal rods just for looks!

frame_thickness=6;   				// 6mm acrylic
pcb_thickness=2;					    // used for rendering PCB and approximating build volume

//smooth_rod_length=495;  			// determins vertical build height with 508 for 8" 
smooth_rod_length=620;  			

rod_diameter=8;						// might be interesting to have larger rods

platform_hinge_offset=33;

carriage_hinge_offset=22;             // default rostock mini platform

// 27 for mini-platform


// printrbed 155,155 for 6x6
//printbed=[155,155];  				
//printbed_screw_spacing=[146.5,147]; 

// prusa 8x8 platform
printbed=[214,214];
printbed_screw_spacing=[209,209];

tower_radius = printbed[0]*0.9;
rod_length=printbed[0]*1.2;

corner_radius=12;

frame_radius=(tower_radius-rod_diameter/2-corner_radius)/cos(30);
