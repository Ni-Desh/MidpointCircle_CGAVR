#include <GL/glut.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int xc, yc, r;
int x = 0, y;
int p;
bool finished = false;

float colors[8][3] = {
    {1.0, 0.0, 0.0}, // Red for Octant 1
    {0.0, 1.0, 0.0}, // Green for Octant 2
    {0.0, 0.0, 1.0}, // Blue for Octant 3
    {1.0, 1.0, 0.0}, // Yellow for Octant 4
    {1.0, 0.0, 1.0}, // Magenta for Octant 5
    {0.0, 1.0, 1.0}, // Cyan for Octant 6
    {1.0, 0.5, 0.0}, // Orange for Octant 7
    {0.5, 0.0, 0.5}  // Purple for Octant 8
};

void renderText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

void drawArrowhead(float x, float y, float angle) {
    float size = 15.0f;
    float angleOffset = 0.4f;

    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x - size * cos(angle - angleOffset),
               y - size * sin(angle - angleOffset));

    glVertex2f(x, y);
    glVertex2f(x - size * cos(angle + angleOffset),
               y - size * sin(angle + angleOffset));
    glEnd();
}

void drawLineWithBothArrows(float x1, float y1, float x2, float y2) {
    float angle = atan2(y2 - y1, x2 - x1);
    float oppositeAngle = angle + M_PI;

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    drawArrowhead(x2, y2, angle);
    drawArrowhead(x1, y1, oppositeAngle);
}

void plotPoint(int x, int y, int octant) {
    glColor3f(colors[octant][0], colors[octant][1], colors[octant][2]);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
    printf("Point: (%d, %d)\n", x, y);
    usleep(10000);
}

void plotAxisLines() {
    char label[20];
    glColor3f(1.0, 1.0, 1.0);

    drawLineWithBothArrows(-500, 0, 500, 0);
    drawLineWithBothArrows(0, -500, 0, 500);
    drawLineWithBothArrows(-500, -500, 500, 500);
    drawLineWithBothArrows(-500, 500, 500, -500);

    glBegin(GL_LINES);
    for (int i = -400; i <= 400; i += 100) {
        if (i != 0) {
            glVertex2i(i, -5);
            glVertex2i(i, 5);
            glVertex2i(-5, i);
            glVertex2i(5, i);
        }
    }
    glEnd();

    for (int i = -400; i <= 400; i += 100) {
        if (i != 0) {
            snprintf(label, sizeof(label), "%d", i);
            renderText(i - 15, -25, label);
            renderText(-35, i - 5, label);
        }
    }

    // Axis labels
    renderText(460, -25, "X");
    renderText(-15, 460, "Y");
    renderText(350, 350, "y = x");
    renderText(350, -350, "y = -x");

    // Octant coordinate labels
    renderText(200, 300, "(x,y)");     // Octant 1 (x,y)
    renderText(-250, 300, "(-x,y)");   // Octant 2 (-x,y)
    renderText(200, -300, "(x,-y)");   // Octant 3 (x,-y)
    renderText(-250, -300, "(-x,-y)"); // Octant 4 (-x,-y)
    renderText(300, 200, "(y,x)");     // Octant 5 (y,x)
    renderText(-300, 200, "(-y,x)");   // Octant 6 (-y,x)
    renderText(300, -200, "(y,-x)");   // Octant 7 (y,-x)
    renderText(-300, -200, "(-y,-x)"); // Octant 8 (-y,-x)


}

void drawCircleStep() {
    if (finished) return;
    plotPoint(xc + x, yc + y, 0);
    plotPoint(xc - x, yc + y, 1);
    plotPoint(xc + x, yc - y, 2);
    plotPoint(xc - x, yc - y, 3);
    plotPoint(xc + y, yc + x, 4);
    plotPoint(xc - y, yc + x, 5);
    plotPoint(xc + y, yc - x, 6);
    plotPoint(xc - y, yc - x, 7);
    x++;
    if (p < 0) {
        p += 2 * x + 1;
    } else {
        y--;
        p += 2 * (x - y) + 1;
    }
    if (x > y) finished = true;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    plotAxisLines();
    while (!finished) {
        drawCircleStep();
    }
    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPointSize(3.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-500, 500, -500, 500, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    y = r;
    p = 1 - r;
}

int main(int argc, char **argv) {
    do {
        printf("Enter circle center coordinates (xc yc): ");
        scanf("%d %d", &xc, &yc);
        printf("Enter radius (max 400): ");
        scanf("%d", &r);
        if (r > 400) {
            printf("Radius must be 400 or less. Please try again.\n");
        }
    } while (r > 400);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Midpoint Circle Algorithm Visualization");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}



