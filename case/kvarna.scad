	radLED = 21.8;

module top()
{
	difference()
	{	
		translate([0,0,10])
		{
			cylinder(r=radLED, h=70, $fn=20);
		}

		translate([0,0,10])
		{
			translate([-24/2,-27/2,1.5])
				cube([24,27,61]);
			translate([18,0,0])
				cylinder(r=1.5, h=22,$fn=16);
			translate([-18,0,0])
				cylinder(r=1.5, h=22,$fn=16);
		}

		translate([1,-29/2, 2.5])
		{
			translate([0,29/2 - 12.5/2,1])
				cube([5,12.5,20]);		//usb
		}

		translate([18.5/2,0,0])
		{
			cylinder(r=1.5, h=12, $fn=16);
		}
		translate([-18.5/2,0,0])
		{
			cylinder(r=1.5, h=12, $fn=16);
		}


		rotate(117,[0,0,1])
			translate([0,-5/2,9])
			{
				translate([20.4,0,0])
				{
					cube([2,5,60]);
					translate([-1.5,0,-4])
						rotate(-60,[0,1,0])
							cube([20,5,3]);
				}
			}

		translate([0,0,45])
			cylinder(r=radLED-3, h=70, $fn=20);
		translate([0,0,25])
			cylinder(r1=10,r2=radLED-3, h=20, $fn=20);
	}
}

module middle()
{
	difference()
	{
		cylinder(r=radLED, h=10, $fn=20);	

		translate([1,-29/2, 2.5])
		{
			cube([1.8, 29, 7.5]);	//pcb
			translate([0,29/2 - 12.5/2,1])
				cube([5.2,12.5,20]);		//usb
			translate([-2,29/2 - 15/2, 4])
				cube([2,15,4]);
		}

		translate([18,0,0])
		{
			cylinder(r=1.65, h=22,$fn=16);
			cylinder(r=3.3, h=3);
		}
		translate([-18,0,0])
		{
			cylinder(r=1.65, h=22,$fn=16);
			cylinder(r=3.3, h=3);
		}

		cylinder(r=6,h=11);

		translate([18.5/2,0,0])
		{
			cylinder(r=1.5, h=11, $fn=16);
			cylinder(r1=1.7, r2=1.5, h=1.5, $fn=16);
			//translate([0,0,10-3])
			//	rotate(30, [0,0,1])
			//	cylinder(r=6.5/2, h=4, $fn=6);
		}
		translate([-18.5/2,0,0])
		{
			cylinder(r=1.5, h=11, $fn=16);
			cylinder(r1=1.7, r2=1.5, h=1.5, $fn=16);
			//translate([0,0,10-3])
			//	rotate(30, [0,0,1])
			//	cylinder(r=6.5/2, h=4, $fn=6);
		}
		rotate(117,[0,0,1])
			translate([0,-5/2,9])
			{
				cube([30,5,1]);
				translate([0,0,-8])
				rotate(-45,[0,1,0])
					cube([20,5,3]);

			}
	}
}

module bottom()
{
	difference()
	{
		union()
		{
			translate([0,0,-7])
			{
				cylinder(r=29/2, h=5);
				cylinder(r1=30.2/2, r2=29/2, h=1.5);
			}
			translate([0,0,-7-2])
			{
				cylinder(r=42/2, h=2);
			}
		}

		translate([0,0,-9])
			cylinder(r=6,h=9);

		translate([18.5/2,0,-9])
		{
			cylinder(r=1.65, h=9, $fn=16);
			cylinder(r=3.3, h=3);
			translate([0,0,3])
				cylinder(r1=3.3, r2=1.65, h=1);
		}
		translate([-18.5/2,0,-9])
		{
			cylinder(r=1.65, h=9, $fn=16);
			cylinder(r=3.3, h=3);
			translate([0,0,3])
				cylinder(r1=3.3, r2=1.65, h=1);
		}


		//pcb loch
		translate([18.5/2,-5.5,-9])
		{
			cylinder(r=1.6, h=9, $fn=16);
			translate([0,0,4])
				rotate(60, [0,0,1])
				cylinder(r=6.5/2, h=3, $fn=6);
		}

		//pcb loch
		translate([-18.5/2,-5.5,-9])
		{
			cylinder(r=1.6, h=9, $fn=16);
			translate([0,0,4])
				rotate(60, [0,0,1])
				cylinder(r=6.5/2, h=3, $fn=6);
		}


		translate([15,-4,-8])
			cube([7,8,1]);
		translate([-15-7,-4,-8])
			cube([7,8,1]);


		translate([-2.5, 0, -9])
			cube([5,25,1.25]);
	}
}

top();
//middle();
//bottom();

