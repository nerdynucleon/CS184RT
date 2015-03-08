class RGB{
  public:
    float r; float g; float b;
    RGB(float, float, float);
};

class Light{
  float intensity;
  RGB color;
  public:
    Light(float, float, float, float);
    Light(float, RGB*);
};