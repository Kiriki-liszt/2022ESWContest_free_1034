import processing.serial.*;
 
Serial	myPort;
Serial	myPort2;

int		rad = 0;
int		rad2 = 0;
float	cx,		cy;
ArrayList<Ball> balls = new ArrayList<Ball>();
 
void setup() {
	size(1400, 1100);
	cx = width/2;
	cy = height*7/15;
	noFill();		//for MAC
	println(Serial.list());
	myPort	= new Serial(this, Serial.list()[8], 115200);
	myPort2	= new Serial(this, Serial.list()[9], 115200);
}
 
void draw(){
	background(220);		// bight gray background
	//arc(cx, cy, width, height, PI, TWO_PI, CHORD);
	//square(100, 100, 300);

	stroke(255, 0, 0);

	square(cx, cy-500, 500);
	square(cx-1050, cy-500, 500);

	rect(cx-100, cy-350, 50, 300);
	rect(cx-200, cy-350, 50, 300);
	rect(cx-300, cy-350, 50, 300);
	rect(cx-400, cy-350, 50, 300);
	rect(cx-500, cy-350, 50, 300);

	rect(cx+50, cy+50,  300, 50);
	rect(cx+50, cy+150, 300, 50);
	rect(cx+50, cy+250, 300, 50);
	rect(cx+50, cy+350, 300, 50);

	fill(255);

	translate(cx, cy, cx);

	//rotateY(map(mouseX, 0, width, 0, PI));
	//rotateZ(map(mouseY/4, 0, height, 0, -PI));
	//camera(100.0, 100.0, 800.0, 100.0, 50.0, 200.0, 0.0, 2.0, 0.0);
	//rotateX(-PI/6);
	//rotateY(PI/3);
	//camera(width/2.0, height/2.0, (height/2.0) / tan(PI*30.0 / 180.0), width/2.0, height/2.0, 0, 0, 1, 0);
	//noFill();
	//box(300, 50, 200);
	//fill(100,100,255);
	//box(30, 5, 20);

	float	r	= TWO_PI-map(	rad,	0,	360,	0,	TWO_PI);
	line(cx,	cy,	cx + (width/2)*cos(r),	cy + (height/2)*sin(r));

	float	r2	= TWO_PI-map(	rad2,	0,	360,	0,	TWO_PI);
	line(cx,	cy,	cx + (width/2)*cos(r2),	cy + (height/2)*sin(r2));

	updateBalls();
	displayBalls();
}
 
void updateBalls(){
	for(int i = balls.size()-1 ; i > -1 ; i--){
		balls.get(i).update();
		if(balls.get(i).isDead())
				balls.remove(i);
		}
}
 
void displayBalls(){
	for(int i = 0 ; i < balls.size() - 1 ; i++){
		balls.get(i).display();
	}
}
 
void serialEvent(Serial p){
	if(p == myPort){
	String inString = p.readStringUntil('\n');
	if(inString != null){
		if(inString.startsWith("r")){
			String[] strings = inString.trim().replace("r","").split("d");
				if(strings.length > 1){
					rad = Integer.parseInt(strings[0]);
					int distance = Integer.parseInt(strings[1]);
					if(distance != 0){
						//println(rad+' '+distance);
						balls.add(new Ball(cx, cy, rad, 2*distance));
					}
				}
		 }
	}
	}
	
	//input 2
	if(p == myPort2){
	String inString2 = p.readStringUntil('\n');
	if(inString2 != null){
		if(inString2.startsWith("r")){
			String[] strings2 = inString2.trim().replace("r","").split("d");
				if(strings2.length > 1){
					rad2 = Integer.parseInt(strings2[0]);
					int distance = Integer.parseInt(strings2[1]);
					if(distance != 0){
						//println(rad2+' '+distance);
						balls.add(new Ball(cx, cy, rad2, distance));
					}
				}
		 }
	}
	}
}
 
class Ball{
	int				life = 250;
	public float	x,	y;
	public Ball(float cx, float cy, int rad, int distance) {
		float			d = map			(distance,	0,		45,		0,	width/2);
		float			r = TWO_PI-map	(rad,		0,		360, 	0,	TWO_PI);
		x = cx + cos(r)*d;
		y = cy + sin(r)*d;
	}
	public void display() {
		//println(Math.round(x),Math.round(y));
		ellipse(x,y, 2, 2);
		//size(400, 400, P3D);
		//lights();
		//translate(x, 0, y);
		//sphere(12);
	}
	public void update() {
		--life;
		if (life < 0) {
		life = 0;
		}
	}
	public boolean isDead(){
		return life == 0;
	}
}
