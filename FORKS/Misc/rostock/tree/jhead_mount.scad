include <jhead.scad>

cone_height=16;
holding_plate_height=4.6;

cone_bottom=20 + 2.5;
cone_top=10;
cone_wall=8;

bore_displacement=cone_top + (cone_wall/2);

module jhead_mount() {
  intersection() {
    difference() {
      // outer
      cylinder(cone_height, cone_bottom + cone_wall, cone_top + cone_wall, center=true, $fn=50);
      // inner
      cylinder(cone_height, cone_bottom, cone_top, center=true, $fn=50);

  	  // triangular center cut
      translate([0,0,6])
      	rotate([0, 0, 60])
        cylinder(5, r=cone_top+10, center=true, $fn=3);
      
      // bore
      for (a = [0, 120, 240]) {
        rotate([0, 0, a])
          translate([bore_displacement, 0, 5])
            cylinder(10, r=1.3, center=true, $fn=20);
      }
    }

    // cutouts
    for (a = [0, 120, 240]) {
      rotate([0, 0, a])
        translate([10, 0, 3])
          cube([cone_bottom + cone_wall + 5, 10, cone_height], center=true);
    }
  }
}

module jhead_holdingplate() {
  intersection() {  
    difference() {
      cylinder(holding_plate_height, r=cone_top + cone_wall, center=true, $fn=50);
      cylinder(holding_plate_height, r=6, center=true, $fn=50);

      rotate([0, 0, 60])
        translate([10, 0, 0])
          cube([20, 12, 6], center=true);

      // bore holes
      for (a = [0, 120, 240]) {
        rotate([0, 0, a])
          translate([bore_displacement, 0, 5])
            cylinder(20, r=1.5, center=true, $fn=25);
      }
    }
    // outer triangular boundary
    cylinder(5, r=cone_top + cone_wall + 8, center=true, $fn=3);
  }
}


// top end
//
module jhead_topend() {
  intersection() {  
    difference() {
      cylinder(10, r=cone_top + cone_wall, center=true, $fn=50);

      union() {
        cylinder(10, r=2, center=true, $fn=25); // M5 Pneumatic
        translate([0, 0, -5.2 / 2])
          cylinder(4.8, r=8.3, center=true, $fn=25); // 16mm Hotend outer dia
      }

      // bores
      for (a = [0, 120, 240]) {
        rotate([0, 0, a])
          translate([bore_displacement, 0, 5])
            cylinder(20, r=1.6, center=true, $fn=25);
      }

      // chamfers
      for (a = [0, 120, 240]) {
        rotate([0, 0, a])
          translate([bore_displacement, 0, 2])
            union() {
             cylinder(7, r=3, center=true, $fn=25);
             translate([7.8, 0, 2.5])
               cube([12,12,12], center=true);
           }
      }
    }
    cylinder(10, cone_top + cone_wall + 8, 9, center=true, $fn=3);
  }
}


//translate([0, 0, 30])
//  jhead_topend();


//translate([0, 0, 20])
//  jhead_holdingplate();

/*
//cylinder(r=20, h=5, center=true);
translate([0, 0, 8])
  jhead_mount();
*/

/*
translate([0, 0, 27])
  rotate([180,0,0])
    hotend_jhead();
*/