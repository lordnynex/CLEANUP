include <configuration.scad>

ringToInlet=7.0;
ringToPlatform=16.3;
ringInnerRadius=14 / 2;

upperHeight=20.5;
upperRadius=16 / 2 + 0.2;

lowerHeight=3.0; // above platform
lowerRadius=20.2 / 2 + 0.2;

//totalHeight = upperHeight + lowerHeight;

module torus(inner, outer) {
  r = (outer - inner ) / 2;
  rotate_extrude(convexity = 10, $fn=100)
    translate([inner + r, 0, 0])
      circle(r, $fn = 25, center=true);
}

// -------------------------------

module nozzle() {
  offset=1.5/2;
  ringSelfRadius=4.5;

  union() {
    // upper chamfer, h=1.5mm
    translate([0, 0, upperHeight - offset])
       cylinder(1.5, upperRadius, upperRadius - 1.5, center=true);
      
     difference() {
       translate([0, 0, upperHeight / 2 - offset ])
         cylinder(r=upperRadius, h=upperHeight - offset*2, center=true, $fn=100);

       translate([0, 0, upperHeight - ringToInlet - offset])
         torus(ringInnerRadius + 0.2, ringInnerRadius + ringSelfRadius);
    }

    // lower chamfer (on nozzle, only 1.5mm)
    translate([0, 0, -lowerHeight / 2])
      cylinder(3, upperRadius+3, upperRadius+0.5, center=true);
  }
}


h = 20.5 + 3.0 + 3.0; // upper, lower, upper top

translate([0, 0, h / 2])
difference() {
  union() {
    // outer main cylinder
    cylinder(r=lowerRadius, h=h, center=true, $fn=100);

    difference() {
      intersection() {
        cube([60, 10, h], center=true);
        cylinder(r=30, h=h, center=true, $fn=100);
      }

      // side cutaways
      for(a=[24, -24])
        translate([a, 0, 6])
          cube([25, 10, h], center=true);

      // 4mm mounting holes
      for(a = [-25, 25]) {
        translate([a, 0, 0])
          cylinder(r=2, h=h, center=true, $fn=20);
      }
    }
  }

  translate([0, 0, -h/2 + 1.5*2])
    nozzle();

  // hole for pneumatic fitting
  translate([0, 0, upperHeight/2])
    cylinder(r=4.13/2, h=10, center=true, $fn=25);

  // cutout to insert nozzle
  translate([0, ringInnerRadius, -1.5*2])
    cube([ringInnerRadius*2+2, 10, h], center=true);

  // side cutaway
  translate([0, 10, 0])
    cube([60, 10, h], center=true);
}









