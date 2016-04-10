include <configuration.scad>
use <carriage.scad>

h=platform_thickness;

//cutout = 12.5;
inset = 6;


cone_height=3.0;
m3_washer_width=0.55;
m3_screw_head_dia = 5.5;
m3_screw_head_h = 2.3;
m3_nut_width = 2.3;
ball_collar_width = 8.2;
cutout = ball_collar_width + 2*m3_washer_width + 2*cone_height; 


module platform() {
  translate([0, 0, h/2]) 
  difference() {
    union() {
      for (a = [0:120:359]) {
        rotate([0, 0, a]) {
          translate([0, -platform_hinge_offset, 0]) parallel_joints();
          // Close little triangle holes.
          translate([0, 31, 0]) cylinder(r=5, h=h, center=true);
          // Holder for adjustable bottom endstops.
          translate([0, 45, 0]) cylinder(r=5, h=h, center=true);
        }
      }
      cylinder(r=30, h=h, center=true);
    }

	// center round cutout
    rotate([0,0,30])
	    difference() {
		    cylinder(r=20, h=h+12, center=true);

            // nozzle supports
			difference() {
		  		cube([40, 10, h+12], center=true);
		  		cylinder(r=11, h=h+12, center=true);
                 for (a = [-15.75, 15.75])
                   translate([a,0,0])
  		             cylinder(r=1.5, h=h+12+10, center=true, $fn=12);		
        		}		
	}

    for (a = [0:2]) {
      rotate(a*120) {
        translate([0, -25, 0])
          cylinder(r=2.2, h=h+1, center=true, $fn=12);
        // Screw holes for adjustable bottom endstops.
        translate([0, 45, 0])
          cylinder(r=1.5, h=h+1, center=true, $fn=12);
      }
    }
	// Added extra set of holes 
    rotate([0,0,180]) for (a = [0:2]) {
      rotate(a*120) {
        translate([0, -25, 0])
          cylinder(r=2.2, h=h+1, center=true, $fn=12);
      }
    }
  }
}

platform();



