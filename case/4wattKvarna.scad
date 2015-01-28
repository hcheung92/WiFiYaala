$fn=64;

module fussADD(rot, height)
{
	rotate(a=rot, v=[0,0,1])
		translate([12.75,0,0])
		{
			cylinder(r=3.5, h=height);
		}
}
module fussDIV(rot, height, hole=1.65)
{
	rotate(a=rot, v=[0,0,1])
		translate([12.75,0,0])
		{
			cylinder(r=hole, h=height);
		}
}


module top()
{
	difference()
	{
		union()
		{
			fussADD(0, 2.5);
			fussADD(120, 2.5);
			fussADD(240, 2.5);
			cylinder(r=16.5, h=1);
		}

		fussDIV(0, 2.5);
		fussDIV(120, 2.5);
		fussDIV(240, 2.5);
		cylinder(r=9.25, h=1);
	}
}


module bottom()
{
	difference()
	{
		union()
		{
			translate([0,0,1])
			cylinder(r2=14.65,r1=14.85, h=4.5);
			cylinder(r=20, h=1.75);
		}

		fussDIV(0, 8);
		fussDIV(120, 8);
		fussDIV(240, 8);

//			translate([12.75,0,0])
//				rotate(30, [0,0,1])
//					cylinder(r=6.5/2, h=2.25, $fn=6);


		translate([-4,14.8,1])
			cube([8,7,1]);
		translate([-4,-14.8-7,1])
			cube([8,7,1]);

		cylinder(r=10, h=8);

		translate([-9.7,-1.5,0.75])
			rotate(a=-185, v=[0,1,0])
				#cube([10,3,0.75]);
	}
	
	difference()
	{
		translate([-3-8,-3,0])
			cube([10,6,4]);
		translate([-2,-3,0])
			rotate(a=45, v=[0,1,0])
				cube([3,6,3]);
		translate([-20,-1.5,0])
			cube([20,3,0.75]);
	}

	translate([-3,-3,4])
		cube([2,6,1]);
}


top();
//bottom();