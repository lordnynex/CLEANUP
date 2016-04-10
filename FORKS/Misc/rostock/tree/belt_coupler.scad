include <cogsndogs.scad>

beltType = T2_5;
beltWidth = 7;
toothCount = 10;


intersection() {
  union() {
    translate([0, 0, 3])
      dog_linear(beltType, toothCount, beltWidth, 3);
    translate([0, -5, 0])
      cube([25, 8, 3]);
  }

  translate([0, -3.5, 0])
    cube([23.8, 6.5, 10]);

}

