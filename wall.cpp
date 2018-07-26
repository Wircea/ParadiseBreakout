

class wallType
{
public:
    int posx1;
    int posy1;
    int posx2;
    int posy2;

    float dist1;
    float dist2;

    bool renderThis;
    bool edge1OutsideFov;
    bool edge2OutsideFov;
    bool bothSeen;

    bool p1closerToLeft;
    bool p2closerToLeft;


    textureType* wallTex;
};
