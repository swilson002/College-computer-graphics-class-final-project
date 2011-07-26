#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

GLuint cube, cylinder, horizMonkeySupport, vertMonkeySupport, bar, monkeyBars,
  monkeySide;

GLuint vertPole, horizPole, sphereEnd, swingSide, swingSeat, swingSupport, 
  swingEnd, swing;

GLuint slidePlatform, slideLegLong, slideLegShort, slideStep, slideStepSide,
  slideHandleStep, slideHandleTopSide,slideHandleSupport, 
  slideHandleTopFront, slideHandleBack, slideHandleBottom,  
  slideBottom, slideInc, slideCurvePiece, slideCurve, slideBase, slideLadder, 
  slideHandles, slideHandleSide, slideFront, slide;

GLUquadricObj *sphere, *slideSphere;

GLfloat light_pos[] = {50.00, 100, 0.00,1.00};

double PI = 3.14159265359;

void createCubeList()
{

  int startx, starty, startz, endx, endy, endz;

  startx = starty = startz = -1;
  endx = endy = endz = 1;

  cube = glGenLists(1);
  glNewList(cube, GL_COMPILE);
    glBegin(GL_POLYGON);
      glVertex3d(startx,starty,startz);  //Front
      glVertex3d(endx,starty, startz);
      glVertex3d(endx,starty, endz);
      glVertex3d(startx,starty, endz);
    glEnd();

    glBegin(GL_POLYGON);
      glVertex3d(startx,starty, endz);  //Top
      glVertex3d(endx, starty, endz); 
      glVertex3d(endx, endy, endz);
      glVertex3d(startx,endy,endz);
    glEnd();

    glBegin(GL_POLYGON);
      glVertex3d(startx, endy, endz); //back
      glVertex3d(endx, endy, endz);  
      glVertex3d(endx, endy, startz);
      glVertex3d(startx, endy, startz);
    glEnd();

    glBegin(GL_POLYGON);
      glVertex3d(startx, endy, startz);  //Bottom
      glVertex3d(endx, endy, startz);
      glVertex3d(endx, starty, startz);
      glVertex3d(startx,starty,startz);
    glEnd();

    glBegin(GL_POLYGON);
      glVertex3d(startx,starty,startz); // Side
      glVertex3d(startx, endy, startz);
      glVertex3d(startx, endy, endz);
      glVertex3d(startx, starty, endz);
    glEnd();

    glBegin(GL_POLYGON);
      glVertex3d(endx, starty, startz); // Side
      glVertex3d(endx, endy, startz);
      glVertex3d(endx, endy, endz);
      glVertex3d(endx, starty, endz);
    glEnd();
  glEndList();
}

void createCylinderList(unsigned int smoothness)
{
  int sides = smoothness + 3;
  double angle = 2.0 * PI / sides;
  double curAngle;

  int ztop = 1;
  int zbottom = -1;

  double radius = 1;
  double startx = radius;
  double starty = 0;
  double curx = startx;
  double cury = starty;

  int count, i = 0;
  int zpos = ztop;
  
  cylinder = glGenLists(1);
  glNewList(cylinder, GL_COMPILE);
  /* create the top and bottom */
    do
    {
      curx = startx;
      cury = starty;
      curAngle = angle;
      
      glBegin(GL_POLYGON);
        glVertex3d(curx, cury, zpos);
        for(count = 0; count < sides; count++)
        {
	  curx = radius * cos(curAngle);
	  cury = radius * sin(curAngle);
	  curAngle += angle;
          glVertex3d(curx, cury, zpos);
        }
      glEnd();
    
      zpos = zbottom;
      i++;
    }while( i < 2);


    /* now the sides */

    curx = startx;
    cury = starty;
    curAngle = angle;
    glBegin(GL_QUAD_STRIP);
      glVertex3d(curx, cury, ztop);
      glVertex3d(curx, cury, zbottom);

      for(count = 0; count < sides; count++)
      {
	curx = radius * cos(curAngle);
	cury = radius * sin(curAngle);
	curAngle += angle;
	glVertex3d(curx, cury, ztop);
	glVertex3d(curx, cury, zbottom);
      }
    glEnd();
  glEndList();
}

/* data for the Monkey Bars */

double monkeyLength = 5;
double monkeyWidth = 1.5;
double monkeyThick = .175;
double monkeyHeight = 3.5;
double barThick = .041667;

void createMonkeyBarsParts()
{
  horizMonkeySupport = glGenLists(1);
  glNewList(horizMonkeySupport, GL_COMPILE);
    glPushMatrix();
    glScaled(monkeyLength, monkeyThick, monkeyThick);
    glCallList(cube);
    glPopMatrix();
  glEndList();

  vertMonkeySupport = glGenLists(1);
  glNewList(vertMonkeySupport, GL_COMPILE);
    glPushMatrix();
    glScaled(monkeyThick, monkeyHeight, monkeyThick);
    glCallList(cube);
    glPopMatrix();
  glEndList();

  bar = glGenLists(1);
  glNewList(bar, GL_COMPILE);
    glPushMatrix();
    glScaled(barThick, barThick, monkeyWidth);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  double endCenter = monkeyLength - monkeyThick;
  double topCenter = monkeyHeight - monkeyThick;

  monkeySide = glGenLists(1);
  glNewList(monkeySide, GL_COMPILE);
    glPushMatrix();
    glTranslated(0, topCenter, 0);
    glCallList(horizMonkeySupport);

    glTranslated(endCenter, -topCenter, 0);
    glCallList(vertMonkeySupport);

    glTranslated(-2 * endCenter, 0, 0);
    glCallList(vertMonkeySupport);
    glPopMatrix();

  glEndList();
  
}
double slideLegRadius = .166667;
double slideLegShorth = .5;
double slideLegLongh = 5;
double slideTopLength = 1.5;
double slideWidth = 1;
double slideTopWidth;
double slideTopThick = .083333;
double slideBarrierRad = .083333;
double stepLength = .125;
double stepThick = .0416667;
double handleRadius = .0416667;
double handleHeight = 1;
double stepAngle =10;
double slideAngle =60;
double stepSideThick = .043;
double stepSideHeight = 5.5;
double handleFrontLength;
double slideIncVertF = 6;
double slideIncLength;
double slideCurveVertF;
double slideBottomLength = 1;
double slideThick = .041667;

void createSlideParts()
{
  slideTopWidth = slideWidth + 4*slideBarrierRad;
  slideCurveVertF = 2 * slideLegLongh - 2 * slideLegShorth - slideIncVertF;

  handleFrontLength = (slideTopWidth - slideWidth-handleRadius)/2;

  slidePlatform = glGenLists(1);
  glNewList(slidePlatform, GL_COMPILE);
    glPushMatrix();
    glScaled(slideTopWidth, slideTopThick, slideTopLength);
    glCallList(cube);
    glPopMatrix();
  glEndList();

  slideLegLong = glGenLists(1);
  glNewList(slideLegLong, GL_COMPILE);
    glPushMatrix();
    glRotated(90,1,0,0);
    glScaled(slideLegRadius, slideLegRadius, slideLegLongh);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  slideLegShort = glGenLists(1);
  glNewList(slideLegShort, GL_COMPILE);
    glPushMatrix();
    glRotated(90,1,0,0);
    glScaled(slideLegRadius, slideLegRadius, slideLegShorth);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  slideStepSide = glGenLists(1);
  glNewList(slideStepSide,GL_COMPILE);
    glPushMatrix();
    glRotated(stepAngle,1,0,0);
    glTranslated(0,-stepSideHeight + slideTopThick,stepLength);
    glScaled(stepSideThick, stepSideHeight, stepLength);
    glCallList(cube);
    glPopMatrix();
  glEndList();

  slideStep = glGenLists(1);
  glNewList(slideStep, GL_COMPILE);
    glPushMatrix();
    glScaled(slideWidth, stepThick, stepLength);
    glCallList(cube);
    glPopMatrix();
  glEndList();

  slideHandleStep = glGenLists(1);
  glNewList(slideHandleStep, GL_COMPILE);
    glPushMatrix();
    glRotated(stepAngle,1,0,0);
    glRotated(-90,1,0,0);
    glTranslated(0,0,0-stepSideHeight);
    glScaled(handleRadius, handleRadius, stepSideHeight);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  slideHandleTopSide = glGenLists(1);
  glNewList(slideHandleTopSide, GL_COMPILE);
    glPushMatrix();
    glScaled(handleRadius, handleRadius, slideTopLength - handleRadius);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  slideHandleSupport = glGenLists(1);
  glNewList(slideHandleSupport, GL_COMPILE);
    glPushMatrix();
    glRotated(90,1,0,0);
    glScaled(handleRadius, handleRadius, handleHeight);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  slideHandleTopFront = glGenLists(1);
  glNewList(slideHandleTopFront, GL_COMPILE);
    glPushMatrix();
    glRotated(90,0,1,0);
    glScaled(handleRadius, handleRadius, slideTopWidth - handleRadius);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  slideHandleBack = glGenLists(1);
  glNewList(slideHandleBack, GL_COMPILE);
    glPushMatrix();
    glRotated(90,0,1,0);
    glScaled(handleRadius, handleRadius, 
	     handleFrontLength);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  double angle = stepAngle * PI / 180;
  double dist = 2*handleHeight * sin(angle)-.07;

  unsigned int curveSmooth = 20;

  slideHandleBottom = glGenLists(1);
  glNewList(slideHandleBottom, GL_COMPILE);
    glPushMatrix();
    glRotated(stepAngle,1,0,0);
    glTranslated(0,0,dist);
    glScaled(handleRadius, handleRadius, dist);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  double phiD, phi;
  if(curveSmooth != 0)
  {
    phiD = (slideAngle / (curveSmooth));
    phi = phiD * PI / 180;
  }
  else
  {
    phiD = slideAngle;
    phi = slideAngle * PI / 180;
  }

  double slideAngleR = slideAngle * PI / 180;

  slideIncLength = slideIncVertF / sin(slideAngleR) / 2;
  slideInc = glGenLists(1);
  glNewList(slideInc, GL_COMPILE);
    glPushMatrix();
    glPushMatrix();
    glTranslated(0,0-slideThick, slideIncLength);
    glScaled(slideWidth, slideThick, slideIncLength);
    glCallList(cube);
    glPopMatrix();
    glPushMatrix();
    glTranslated(slideWidth + slideBarrierRad, 0, slideIncLength);
    glScaled(slideBarrierRad, slideBarrierRad, slideIncLength);
    glCallList(cylinder);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0-slideWidth - slideBarrierRad, 0, slideIncLength);
    glScaled(slideBarrierRad, slideBarrierRad, slideIncLength);
    glCallList(cylinder);
    glPopMatrix();

    glPopMatrix();
  glEndList();

  /* calculating curve piece length */
  // y = R-Rcos(a);


  double radius = slideCurveVertF / (1 - cos(slideAngleR));
  double angle2 = (180 - slideAngleR) / 2;

  // get length using law of sines
  double curveLengthF = radius * sin(phi) / sin(angle2);
  double curveLength = curveLengthF / 2;

  slideCurvePiece = glGenLists(1);
  glNewList(slideCurvePiece, GL_COMPILE);
    glPushMatrix();
    glTranslated(0,0,curveLength);

    glPushMatrix();
    glTranslated(0,0-slideThick,0);
    glScaled(slideWidth, slideThick, curveLength);
    glCallList(cube);
    glPopMatrix();

    glPushMatrix();
    glTranslated(slideWidth + slideBarrierRad, 0, 0);
    glScaled(slideBarrierRad, slideBarrierRad, curveLength);
    glCallList(cylinder);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0-slideWidth - slideBarrierRad, 0,0);
    glScaled(slideBarrierRad, slideBarrierRad, curveLength);
    glCallList(cylinder);
    glPopMatrix();
    
    glPopMatrix();

    // push the rest out into position

    glTranslated(0,0,curveLengthF);
    glRotated(0-phiD,1,0,0);
  glEndList();

  int count = 0;
  slideCurve = glGenLists(1);
  glNewList(slideCurve, GL_COMPILE);

    glRotated(0-phiD,1,0,0);
    for(count = 0; count < curveSmooth; count++)
    {
      glCallList(slideCurvePiece);
    }

  glEndList();

  slideBottom = glGenLists(1);
  glNewList(slideBottom, GL_COMPILE);
    glPushMatrix();
    glTranslated(0,0,slideBottomLength);
    glPushMatrix();
    glTranslated(0,0-slideThick, 0);
    glScaled(slideWidth, slideThick, slideBottomLength);
    glCallList(cube);
    glPopMatrix();

    //barriers
    glPushMatrix();
    glTranslated(slideWidth + slideBarrierRad, 0, 0);
    glScaled(slideBarrierRad, slideBarrierRad, slideBottomLength);
    glCallList(cylinder);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0-slideWidth - slideBarrierRad, 0, 0);
    glScaled(slideBarrierRad, slideBarrierRad, slideBottomLength);
    glCallList(cylinder);
    glPopMatrix();

    //legs

    glPopMatrix();

    double zTran = slideBottomLength *3/2;
    glPushMatrix();
    
    glTranslated(slideWidth-slideBarrierRad,0-slideLegShorth,zTran);
    glRotated(90,1,0,0);
    glScaled(slideLegRadius, slideLegRadius, slideLegShorth);
    glCallList(cylinder);
    glPopMatrix();

    glPushMatrix();
    
    glTranslated(0-slideWidth+slideBarrierRad,0-slideLegShorth, zTran);
    glRotated(90,1,0,0);
    glScaled(slideLegRadius, slideLegRadius, slideLegShorth);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();
}

void createSlideSections()
{
  double slideLegOffCenter = slideLegRadius + .05;
  double legPosx = slideTopWidth - slideLegOffCenter;
  double legPosz = slideTopLength - slideLegOffCenter;

  slideSphere = gluNewQuadric();

  slideBase = glGenLists(1);
  glNewList(slideBase, GL_COMPILE);
    glPushMatrix();

    //platform
    glColor3f(0,0,1);
    glCallList(slidePlatform);

    //legs
    glTranslated(0,0-slideLegLongh,0);

    glPushMatrix();
    glTranslated(legPosx,0,legPosz);
    glCallList(slideLegLong);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0-legPosx,0,legPosz);
    glCallList(slideLegLong);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0-legPosx,0,0-legPosz);
   glCallList(slideLegLong);
    glPopMatrix();

    glPushMatrix();
    glTranslated(legPosx,0,0-legPosz);
    glCallList(slideLegLong);
    glPopMatrix();
    glPopMatrix();
  glEndList();

  double angle = stepAngle * PI / 180;  //convert to radians

  double reflect[] = {-1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
  slideHandleSide = glGenLists(1);
  glNewList(slideHandleSide, GL_COMPILE);
    glPushMatrix();

    glColor3f(.5,.5,.5);
    glCallList(slideHandleTopSide);
    glTranslated(0,0,slideTopLength - handleRadius);

    gluSphere(slideSphere, handleRadius, 10, 10);

    glTranslated(0, 0-handleHeight,0);
    glCallList(slideHandleSupport);

    glTranslated(0,0,-2*(slideTopLength - handleRadius));
    glCallList(slideHandleSupport);

    glTranslated(0,handleHeight,0);
    gluSphere(slideSphere, handleRadius, 10, 10);

    glTranslated(handleFrontLength,0,0);
    glCallList(slideHandleBack);

    glTranslated(handleFrontLength,0,0);
    gluSphere(slideSphere, handleRadius,10,10);
    glCallList(slideHandleStep);

    glTranslated(0,(0-stepSideHeight) * cos(angle), 
		 (0-stepSideHeight) * sin(angle));
    glCallList(slideHandleBottom);
    glTranslated(0,(0-stepSideHeight) * cos(angle), 
		 (0-stepSideHeight) * sin(angle));
    gluSphere(slideSphere, handleRadius, 10,10);
    glCallList(slideHandleBottom);

    glPopMatrix();
  glEndList();

  slideHandles = glGenLists(1);
  glNewList(slideHandles, GL_COMPILE);
    glPushMatrix();
    glTranslated(0-slideTopWidth + handleRadius,0,0);
    glCallList(slideHandleSide);

    glTranslated(slideTopWidth - handleRadius,0,slideTopLength - handleRadius);
    glCallList(slideHandleTopFront);

    glTranslated(slideTopWidth - handleRadius,0,
		 0-slideTopLength + handleRadius);
    glMultMatrixd(reflect);
    glCallList(slideHandleSide);

    glPopMatrix();
  glEndList();

  double stepDif = 1;

  double stepDifz = stepDif * sin(angle);
  double stepDify = stepDif * cos(angle);
  int count;

  slideLadder = glGenLists(1);
  glNewList(slideLadder, GL_COMPILE);
    glColor3f(0,0,1);

    glPushMatrix();
    glTranslated(slideWidth, 0, 0);
    glCallList(slideStepSide);
    glTranslated(-2*(slideWidth),0,0);
    glCallList(slideStepSide);
    glPopMatrix();

    glPushMatrix();

    for(count = 0; count < 9; count++)
    {
      glTranslated(0,0-stepDify, 0-stepDifz);
      glCallList(slideStep);
    }
    glPopMatrix();
  glEndList();

  slideFront = glGenLists(1);
  glNewList(slideFront, GL_COMPILE);
    glPushMatrix();
    glColor3f(.5,.5,.5);
    glRotated(slideAngle,1,0,0);
    glCallList(slideInc);

    glTranslated(0,0,slideIncLength * 2);

    glCallList(slideCurve);
    glCallList(slideBottom);
    glPopMatrix();
  glEndList();
}

double poleLength = 8.5;
double setLength = 8;
double poleRadius = .125;
double stringRadius = .0208333;
double stringLength = 4.5;
double seatLength = 1;
double seatWidth = .5;
double seatThick = .041667;

void createSwingParts()
{
  sphere = gluNewQuadric();

  sphereEnd = glGenLists(1);
  glNewList(sphereEnd, GL_COMPILE);
    gluSphere(sphere,poleRadius,10,10);
  glEndList();

  horizPole = glGenLists(1);
  glNewList(horizPole, GL_COMPILE);
    glPushMatrix();
    glScaled(poleRadius, poleRadius, setLength);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  vertPole = glGenLists(1);
  glNewList(vertPole, GL_COMPILE);
    glPushMatrix();
    glTranslated(0,0-poleLength,0);
    glRotated(90, 1,0,0);
    glScaled(poleRadius, poleRadius, poleLength);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  swingSide = glGenLists(1);
  glNewList(swingSide,GL_COMPILE);
    glPushMatrix();
    glRotated(90,1,0,0);
    glScaled(stringRadius, stringRadius, stringLength);
    glCallList(cylinder);
    glPopMatrix();
  glEndList();

  swingSeat = glGenLists(1);
  glNewList(swingSeat, GL_COMPILE);
    glPushMatrix();
    glScaled(seatWidth, seatThick, seatLength);
    glCallList(cube);
    glPopMatrix();
  glEndList();
  
  swingEnd = glGenLists(1);
  glNewList(swingEnd, GL_COMPILE);
    glPushMatrix();

    glTranslated(0,0,setLength);

    glCallList(sphereEnd);

    glPushMatrix();
    glRotated(-30,0,0,1);
    glCallList(vertPole);

    glRotated(60,0,0,1);
    glCallList(vertPole);
    glPopMatrix();

    glRotated(-30,1,0,0);
    glCallList(vertPole);

    glPopMatrix();
  glEndList();
}

void initDisplayLists()
{
  double endCenter = monkeyLength - monkeyThick;
  int count;
  createCubeList();
  createCylinderList(7);

  glMatrixMode(GL_MODELVIEW);

  createSlideParts();
  // pliminary

  createSlideSections();

  slide = glGenLists(1);
  glNewList(slide, GL_COMPILE);
    glPushMatrix();

    glCallList(slideHandles);

    glTranslated(0,-2*handleHeight,0);

    glCallList(slideBase);

    glTranslated(0,0,0-slideTopLength);

    glCallList(slideLadder);

    glTranslated(0,0,2*slideTopLength);

    glCallList(slideFront);

    glPopMatrix();
  glEndList();

  createSwingParts();

  //swing frames
  swingSupport = glGenLists(1);
  glNewList(swingSupport, GL_COMPILE);
    glPushMatrix();
    glColor3f(1,0,1);
    glCallList(horizPole);

    glPushMatrix();
    glRotated(30,0,0,1);
    glCallList(vertPole);

    glRotated(-60,0,0,1);
    glCallList(vertPole);

    glPopMatrix();

    glCallList(swingEnd);

    glRotated(180,0,1,0);
    glCallList(swingEnd);

    
    glPopMatrix();
  glEndList();

  // swing seats, separate to allow animation
  swing = glGenLists(1);
  glNewList(swing, GL_COMPILE);
    glPushMatrix();

    glColor3f(.5,.5,.5);
    glTranslated(0,-1*stringLength - poleRadius, seatLength);
    glCallList(swingSide);

    glTranslated(0,0,-2*seatLength);
    glCallList(swingSide);

    glColor3f(1,0,0);
    glTranslated(0,-1*stringLength, seatLength);
    glCallList(swingSeat);
    glPopMatrix();
  glEndList();
  
  // monkeybars
  createMonkeyBarsParts();

  monkeyBars = glGenLists(1);
  glNewList(monkeyBars, GL_COMPILE);

    glColor3f(.65,.5,.4);
    glPushMatrix();
    glTranslated(0,0-monkeyHeight,0);

    glPushMatrix();
    glTranslated(0,0,monkeyWidth);
    glCallList(monkeySide);

    glTranslated(0,0,-2*monkeyWidth);
    glCallList(monkeySide);
    glPopMatrix();

    glPushMatrix();
      glTranslated(endCenter,0,0);
      glCallList(bar);
      for(count = 0; count < 3; count++)
      {
        glTranslated(0,-1,0);
        glCallList(bar);
      }

      // other side

      glTranslated(-2 *endCenter,0,0);
      glCallList(bar);
      for(count = 0; count < 3; count++)
      {
        glTranslated(0,1,0);
        glCallList(bar);
      }

      //top

      glTranslated(.5 - monkeyThick ,monkeyHeight - monkeyThick,0);
      glCallList(bar);
      for(count = 0; count < 9; count++)
      {
	glTranslated(1,0,0);
	glCallList(bar);
      }

    glPopMatrix();

    glPopMatrix();
  glEndList();

  
}

int viewMax = 300;
double swingAngleMax[3][4];
double swingCurAngle[3][4];
int field = 100;

double xpos = -50, ypos = 3, zpos = 0;
double posAngle = 0;

void display()
{

  double swingdist = setLength / 3;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(xpos,ypos,zpos,xpos + cos(posAngle),ypos,zpos + sin(posAngle),
	    0,1,0);

  // background

  int size = field + viewMax;

  int sandField = 50;

  // drawing grass
  glColor3f(0,1,0);
  glBegin(GL_POLYGON);
    glVertex3i(0-size, 0, 0-size);
    glVertex3i(0-size, 0, size);
    glVertex3i(size, 0, size);
    glVertex3i(size, 0, 0-size);
  glEnd();

  //drawing sandfield
  glColor3f(1,1,0);
  glBegin(GL_POLYGON);
  // keeping it a little above the grass so it will be seen
    glVertex3f(0-sandField, .02, 0-sandField);
    glVertex3f(0-sandField, .02, sandField);
    glVertex3f(sandField, .02, sandField);
    glVertex3f(sandField, .02, 0-sandField);
  glEnd();

  // drawing slide
  glPushMatrix();
  glTranslated(0, 2 * slideLegLongh + 2*handleHeight,0);

  glTranslated(30,0,0);
  glCallList(slide);

  glTranslated(-60, 0, -40);
  glRotated(45, 0,1,0);
  glCallList(slide);
  glPopMatrix();

  //drawing monkeybars
  glPushMatrix();

  glTranslated(0,2*monkeyHeight,0);

  glTranslated(-35,0,-10);
  glCallList(monkeyBars);

  glTranslated(5,0,30);
  
  glRotated(80,0,1,0);
  glCallList(monkeyBars);

  glPopMatrix();
  

  // drawing swingset
  int swingIndex = 0;
  glPushMatrix();

  glTranslated(0, 2*poleLength / pow(3,.5) - .1,20);

  glTranslated(10,0,-40);
  glPushMatrix();
  glCallList(swingSupport);

  glTranslated(0,0,swingdist);

  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][0]*sin(swingCurAngle[swingIndex][0]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  glTranslated(0,0,swingdist);

  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][1]*sin(swingCurAngle[swingIndex][1]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  glTranslated(0,0,-3*swingdist);
  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][2]*sin(swingCurAngle[swingIndex][2]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  glTranslated(0,0,0-swingdist);
  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][3]*sin(swingCurAngle[swingIndex][3]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  glPopMatrix();
  
  glPopMatrix();

  glPushMatrix();

  glTranslated(0, 2*poleLength / pow(3,.5) - .1,30);

  swingIndex = 1;

  glRotated(90,0,1,0);
  glCallList(swingSupport);


  glTranslated(0,0,swingdist);

  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][0]*sin(swingCurAngle[swingIndex][0]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  glTranslated(0,0,swingdist);

  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][1]*sin(swingCurAngle[swingIndex][1]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  glTranslated(0,0,-3*swingdist);
  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][2]*sin(swingCurAngle[swingIndex][2]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  glTranslated(0,0,0-swingdist);
  glPushMatrix();
  glRotated(swingAngleMax[swingIndex][3]*sin(swingCurAngle[swingIndex][3]),
	    0,0,1);
  glCallList(swing);
  glPopMatrix();

  
  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}


void init()
{
  glClearColor(0,1,1,0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50, 1, 1, viewMax);
  glMatrixMode(GL_MODELVIEW);

  //initalize start and maximum angles for the swing
  // max in degrees, init in radians
  swingAngleMax[0][0] = 45;
  swingAngleMax[0][1] = 85;
  swingAngleMax[0][2] = 60;
  swingAngleMax[0][3] = 90;

  swingCurAngle[0][0] = 0;
  swingCurAngle[0][1] = PI / 3;
  swingCurAngle[0][2] = PI / 2;
  swingCurAngle[0][3] = 14 * PI / 8;

  swingAngleMax[1][0] = 60;
  swingAngleMax[1][1] = 30;
  swingAngleMax[1][2] = 45;
  swingAngleMax[1][3] = 80;

  swingCurAngle[1][0] = 13 * PI/ 7;
  swingCurAngle[1][1] = 4 * PI / 3;
  swingCurAngle[1][2] = PI;
  swingCurAngle[1][3] = 3 * PI / 8;
}

void swingAnim()
{
  double angIncre = .01;
  int count1, count2;
  double twoPi = 2 * PI;

  for(count1 = 0; count1 < 2; count1++)
  {
    for(count2 = 0; count2 < 4; count2++)
    {
      swingCurAngle[count1][count2] += angIncre;
      if(swingCurAngle[count1][count2] >= twoPi)
      {
	swingCurAngle[count1][count2] -= twoPi;
      }
    }
  }

  display();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   gluPerspective(50.0, 1.0, 1.0, viewMax);

   glMatrixMode(GL_MODELVIEW);

}

int fullScreen = 0;

void keyboard(unsigned char key, int x, int y)
{
  double PIOverTwo;
  switch(key)
  {
    case 27: // escape
             exit(0); break;

	     // forward
    case '8':
    case 'w':
    case 'W':  xpos += cos(posAngle); 
               zpos += sin(posAngle); 
               break;

	       // backwards
    case '5':
    case 's':
    case 'S':  xpos -= cos(posAngle);
               zpos -= sin(posAngle); 
	       break;

	       //up
    case '+':
    case 'f':
    case 'F': ypos++; 
      if(ypos > 30)
      { 
	ypos = 30;
      }
      break;

      //down

    case '-':
    case 'r':
    case'R':  ypos--; 
      if(ypos < 1)
      {
	ypos = 1;
      }
    break;

     
    // in this model, the positive direction for angles is clockwise
    // moving left
    case '4':
    case 'a':
    case 'A': PIOverTwo = PI / 2;
              xpos -= cos(posAngle + PIOverTwo);
	      zpos -= sin(posAngle + PIOverTwo); break;

    // moving right
    case '6':
    case 'd':
    case 'D': PIOverTwo = PI / 2;
              xpos += cos(posAngle + PIOverTwo);
	      zpos += sin(posAngle + PIOverTwo); break;

    // turning left
    case '7':
    case 'q':
    case 'Q': posAngle -= PI / 30; 
      if(posAngle < 0) { posAngle += 2 * PI;} break;

    // turning right
    case '9':
    case 'e':
    case 'E': posAngle += PI / 30; 
      if(posAngle > 2 * PI) {posAngle -= 2 * PI;} break;
  }

  //putting boundaries on the field

  if(xpos > field) 
  {
    xpos = field;
  }
  else if (xpos < 0-field) 
  { 
    xpos = 0-field;
  }

  if(zpos > field) 
  {
    zpos = field;
  }
  else if (zpos < 0-field) 
  { 
    zpos = 0-field;
  } 

}

main(int argc, char** argv)
{
    glutInit(&argc, argv);
/* need both double buffering and z buffer */

    char *title = "Scott Wilson Park";

    if(argc > 1)
    {
      title = *(argv + 1);
    }

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(250,50);
    glutInitWindowSize(600, 600);
    glutCreateWindow(title);
    initDisplayLists();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(swingAnim);
    glutKeyboardFunc(keyboard);
    	   glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
    glutMainLoop();
}
