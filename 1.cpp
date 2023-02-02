#include <bits/stdc++.h>
using namespace std;

#include <GL/glut.h>

void display();

class Wiget {
public:
  Wiget(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

  virtual void Draw() = 0;

  virtual void OnLeftPushDown(){
    //cout << "OnLeftPushDown" << endl;
  }

  virtual void OnLeftPushUp(){
    //cout << "OnLeftPushUp" << endl;
  }

  virtual void OnHover(int mx, int my){
    //cout << "OnMouseMove mx:" << mx << " my:" << my << endl;
  }

  virtual void OnKeyDown(int key){
    cout << "OnKeyDown key:" << key << endl;
  }

  virtual void OnFocus(){
    cout << "OnFocus" << endl;
  }

  virtual void OnUnFocus(){
    cout << "OnUnFocus" << endl;
  }

  virtual void OnTimer(){
    //cout << "OnTimer" << endl;
  }

  bool Contains(int mx, int my) {
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
  }

  void drawRect(int x, int y, int w, int h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
  }

  void drawTransparentRect(int x, int y, int w, int h, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
  }

  //虚线矩形
  void drawDashedBorder(int x, int y, int w, int h, int linewidth = 1, float r = 0.2, float g = 0.2, float b = 0.4) {
    glLineWidth(linewidth);
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glVertex2i(x, y + h);
    glVertex2i(x, y);
    glEnd();
  }

  void drawBorder(int x, int y, int w, int h, int linewidth = 3, float r = 0.2, float g = 0.2, float b = 0.4) {
    glLineWidth(linewidth);
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
  }

  void drawText(int x, int y, string text, float r = 0, float g = 0, float b = 0) {
    glColor3f(r, g, b);
    glRasterPos2i(x + 20, y + 20);
    for (char c : text) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
  }

  int calcStringWidth(string text){
    int _width = 0;
    for (char c : text) {
      _width += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    return _width;
  }

  bool isHover = false;
  bool isFocus = false;

protected:
  int x, y, w, h;
};

class Button : public Wiget {
public:
  Button(int x, int y, int w, int h, string text) : Wiget(x, y, w, h), text(text){}

  enum State {
    NORMAL,
    PUSHDOWN
  };

  void Draw() {
    if (state == PUSHDOWN) {
      int offset = 1;
      drawRect(x, y, w, h, 0.8, 0.8, 0.8);
      drawBorder(x, y, w, h);
      drawText(x+offset, y-offset, text);
    } else if (state == NORMAL) {
      drawRect(x, y, w, h, 0.7, 0.7, 0.7);
      drawBorder(x, y, w, h);
      drawText(x, y, text);
    }
    if(isHover){
      drawBorder(x, y, w, h, 5, 0.4, 0.4, 0.7);
    }
    if(isFocus){
      int offset = 7;
      drawDashedBorder(x+offset, y+offset, w-offset*2, h-offset*2, 1, 0.6, 0.6, 0.7);
    }    
  }

  void OnLeftPushDown(){
    state = PUSHDOWN;
  }

  void OnLeftPushUp(){
    state = NORMAL;
    if (onClick != nullptr) {
      onClick();
    }
  }

  void OnHover(int x, int y){
    glutSetCursor(GLUT_CURSOR_INFO);
  }

  void setText(string text){
    this->text = text;
  }

  void setOnClick(void (*onClick)()){
    this->onClick = onClick;
  }

private:
  string text = "Button";
  State state = NORMAL;
  void (*onClick)() = nullptr;
};

class TextBox : public Wiget {
public:
  TextBox(int x, int y, int w, int h, string text) : Wiget(x, y, w, h), 
                                                     text(text)
                                                     {}


  void Draw() {
    drawRect(x, y, w, h, 1, 1, 1);
    drawBorder(x, y, w, h);
    drawText(x, y, text);
    if(isHover){
      drawBorder(x, y, w, h, 5, 0.4, 0.4, 0.7);
    }
    if(isFocus){
      if(showCursor){
        int stringWidth = calcStringWidth(text);
        drawRect(x+20+stringWidth, y+15, 2, 30, 0, 0, 0);
      }
    }
  }

  void OnKeyDown(int key){
    cout << "TextBox OnKeyDown key:" << key << endl;
    if (key == 8) {
      if (text.size() > 0) {
        text.pop_back();
      }
    } else {
      text += key;
    }
  }

  void OnHover(int x, int y){
    glutSetCursor(GLUT_CURSOR_TEXT);
  }

  void OnTimer(){
    showCursor = !showCursor;
    glutPostRedisplay();
  }

  void setText(string text){
    this->text = text;
  }



private:
  string text;
  bool showCursor = true;
};

class Window {
public:
  Window() {}

  Window(int w, int h) : w(w), h(h) {}

  void AddWiget(Wiget *w) {
    wigets.push_back(w);
  }

  void Draw() {
    // Draw window background
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(w, 0);
    glVertex2i(w, h);
    glVertex2i(0, h);
    glEnd();
    for (Wiget *w : wigets) {
      w->Draw();
    }
  }

  void OnTimer() {
    for (Wiget *w : wigets) {
      w->OnTimer();
    }
  }

  void MouseClick(int button, int state, int x, int y) {
    for(Wiget *w : wigets){
      if (w->Contains(x, glutGet(GLUT_WINDOW_HEIGHT) - y - 1)) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
          //focus
          if(foucsWiget){
            foucsWiget->isFocus = false;
            foucsWiget->OnUnFocus();
          }
          foucsWiget = w;
          foucsWiget->isFocus = true;
          foucsWiget->OnFocus();
          w->OnLeftPushDown();
        } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
          w->OnLeftPushUp();
        }
        return;
      }
    }
    if(foucsWiget){
      foucsWiget->isFocus = false;
      foucsWiget->OnUnFocus();
    }
    foucsWiget = nullptr;//unfocus
  }

  void MouseMove(int x, int y) {
    for(Wiget *w : wigets){
      if (w->Contains(x, glutGet(GLUT_WINDOW_HEIGHT) - y - 1)) {
        if(hoverWiget)hoverWiget->isHover = false;
        hoverWiget = w;
        w->isHover = true;
        w->OnHover(x, y);
        return;
      }
    }
    if(hoverWiget)hoverWiget->isHover = false;
    hoverWiget = nullptr;
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
  }

  void KeyDown(int key) {
    if (foucsWiget != nullptr) {
      foucsWiget->OnKeyDown(key);
    }
  }

private:
  int w,h;
  vector<Wiget *> wigets;
  Wiget *foucsWiget = nullptr;
  Wiget *hoverWiget = nullptr;
};

int width = 800;
int height = 500;
Window wnd(width, height);

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  wnd.Draw();
  glFlush();
}

void timer(int value) {
  glutPostRedisplay();
  wnd.OnTimer();
  glutTimerFunc(500, timer, 0);
}

void mouse(int button, int state, int x, int y) {
  wnd.MouseClick(button, state, x, y);
  glutPostRedisplay();
}

void motion(int x, int y) {
  wnd.MouseMove(x, y);
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  wnd.KeyDown(key);
  glutPostRedisplay();
}



void resize(int w, int h) {
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

Button b1(100, 100, 115, 60, "Set");
Button b2(100, 200, 115, 60, "Clear");
Button b3(100, 300, 115, 60, "Exit");
TextBox tb(300, 100, 300, 60, "TextBox");
TextBox tb2(300, 200, 350, 60, "TextBox2");
TextBox tb3(300, 300, 350, 60, "TextBox3");

void Button1OnClick(){
  tb.setText("HelloWorld");
}

void Button2OnClick(){
  tb.setText("");
}

void Button3OnClick(){
  exit(0);
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutCreateWindow("Window and Button");
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutPassiveMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(resize);
  glutTimerFunc(1, timer, 0);

  
  b1.setOnClick(Button1OnClick);
  b2.setOnClick(Button2OnClick);
  b3.setOnClick(Button3OnClick);

  wnd.AddWiget(&b1);
  wnd.AddWiget(&b2);
  wnd.AddWiget(&b3);
  wnd.AddWiget(&tb);
  wnd.AddWiget(&tb2);
  wnd.AddWiget(&tb3);


  glutMainLoop();

  return 0;
}

