// Description: "Tapster - Mobile Automation Robot"
// Project home: http://tapsterbot.com

beam_width     = 8;
hole_diameter  = 5.1;
hole_radius    = hole_diameter / 2;
beam_length = 72;

difference() {
    // Draw the beam...
    union(){
        cube([beam_length,beam_width,beam_width]);

        translate([0,6,0])
        cube([32,4,8]);

        translate([32.0,4.35,0])
        rotate([0,0,45])
        cube([4,4,8]);
    }
    
    rotate([90,0,0])
    translate([16,4,-1])
    cylinder(r=4.2, h=5, $fn=30);

    // Hole #1 for servo
    rotate([90,0,0])
    translate([4,beam_width/2,-12])
    cylinder(r=1.3, h=14, $fn=30);

    // Hole #2 for servo
    rotate([90,0,0])
    translate([28,beam_width/2,-12])
    cylinder(r=1.3, h=14, $fn=30);

    // Hole for connecting to lower arm
    rotate([90,0,0])
    translate([4+8*8,beam_width/2,-10])
    cylinder(r=hole_radius, h=12, $fn=30);
}

