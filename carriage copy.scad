include <configuration.scad>
include <cogsndogs.scad>
include <lm8uu-holder-round-dualscrew.scad>

width = 76;
height = carriage_height;

m3_washer_width=0.51;
m3_nut_width=2.3;
ball_collar_width=8.2;

printed_joints_width=13;

offset = 25;

cone_height=3.0;

cutout = ball_collar_width + 2*m3_washer_width + 2*cone_height; // was: printed_joints_width

middle = 2 * offset - width / 2;


module cone() {
  intersection() {
    difference() {
      cylinder(cone_height, 5.00, 3.25, center=true);
      cylinder(cone_height, 1.55, 1.55, center=true, $fn=12);
    }
  cube([8,10,cone_height], center = true);
  }
}

module parallel_joints(reinforced) {
  difference() {
    union() {
    // cylinder for screws
      intersection() {
        cube([width, 20, 8], center=true);
        rotate([0, 90, 0]) cylinder(r=5, h=width, center=true);
      }

    // angled reinforcements
      intersection() {
        translate([0, 18, 4]) rotate([45, 0, 0])
          cube([width, reinforced, reinforced], center=true);
        translate([0, 0, 20]) cube([width, 35, 40], center=true);
      }

    // forward facing supports where the holes are in
      translate([0, 8, 0]) cube([width, 16, 8], center=true);
    }

  // 3mm screw hole
    rotate([0, 90, 0]) cylinder(r=1.55, h=80, center=true, $fn=12);

    for (x = [-offset, offset]) {
    // inner round cutout
      translate([x, 2.5, 0])
        cylinder(r=cutout/2, h=100, center=true, $fn=24);
      
    // front opening so that it's a fork
      translate([x, -7.5, 0])
        cube([cutout, 20, 100], center=true);
    }

    // center cutout, first round recess, then straighten in front
    translate([0, 2, 0]) cylinder(r=middle, h=100, center=true);  
    translate([0, -8, 0]) cube([2*middle, 20, 100], center=true);

    // inner nut recess
    for (x = [-middle, middle]) {
      translate([x, 0, 0]) rotate([0, 90, 0]) rotate([0, 0, 30])
        cylinder(r=3.3, h=m3_nut_width*2, center=true, $fn=6);
    }
  }

  for (x = [-offset, offset]) {
    translate([x - cutout/2 + 1.5, 0, 0])
      rotate([0, 90, 0])
       cone(3);

    translate([x + cutout/2 - 1.5, 0, 0])
      rotate([0, 270, 0])
       cone(3);
  }
}

module lm8uu_mount(flip) {
  translate([0, -9.4, 0])
  if (flip == true) {
    rotate([270, 0, 0]) lm8uu_holder();
  }
  else {
        rotate([270, 180, 0]) lm8uu_holder();
  }   
}

// height = carriage_height = 24;

module belt_mount() {
  mount_height = 13;
    linear_length = 20.975; // as output after rendering

  intersection() {  
    // linear mount
    translate([10, 8.5, -12])
      rotate([0, 270, 90]) 
        dog_linear(T2_5, 10, mount_height, 4);

    // curved mount
    /*translate([9, 9, 0])
      rotate([90, 180, 0])
        dog_ext(T2_5, 9, 180, mount_height, 3);
    */

    // cut overflow
    translate([9, 2, 0]) 
      cube([25, mount_height, height], center=true);
  }

  // Oririnal mount position for reference
    //translate([8, 2, 0]) 
  //  cube([4, mount_height, height], center=true);
}

module carriage() {
  translate([0, 0, height/2]) 
  union() {
    for (x = [-30, 30]) {
      // ORIG translate([x, 0, 0]) lm8uu_mount(d=15, h=24);
      if (x == 30) {
        translate([x, 0, 0]) lm8uu_mount(false);
      }
      else {
        translate([x, 0, 0]) lm8uu_mount(true);
      }
    }

    belt_mount();

    difference() {
      union() {
        translate([0, -5.6, 0])
          cube([50, 5, height], center=true);
        translate([0, -carriage_hinge_offset, -height/2+4])
          parallel_joints(16);
      }

      // Screw hole for adjustable top endstop.
      translate([15, -16, -height/2+4])
        cylinder(r=1.5, h=20, center=true, $fn=12);

      for (x = [-30, 30]) {
        translate([x, 0, 0])
          cylinder(r=8, h=height+1, center=true);

        // Zip tie tunnels.
        //for (z = [-height/2+4, height/2-4])
        //  translate([x, 0, z])
        //    cylinder(r=13, h=3, center=true);
      }
    }
  }
}

carriage();

// Uncomment the following lines to check endstop alignment.
// use <idler_end.scad>;
// translate([0, 0, -20]) rotate([180, 0, 0]) idler_end();
