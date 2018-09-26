#ifndef SPARKLES_H_INCLUDED
#define SPARKLES_H_INCLUDED
Sprite circle;
Sprite snowflake;
void loadParticles()
{
    circle.init(640,640,"circle.png", true);
    snowflake.init(640,640,"snowflake.png", true);
}
class Particle
{
    Sprite* sprite;
protected:
    glm::vec3 color;
    glm::vec2 coords;
public:
    Particle(glm::vec3 Color, Sprite& s, int x, int y)
    {
        color = Color;
        sprite = &s;
        coords.x = x;
        coords.y = y;
    }
    virtual void update()
    {

    }
    glm::vec3 getColor()
    {
        return color;
    }
    glm::vec2 getCoords()
    {
        return coords;
    }


};

class Snowflake : public Particle
{
    float speed = 0;
public:
    Snowflake(int x, int y) : Particle({1,1,1}, snowflake, x,y )
    {
        speed = (rand()%100)/100.0;
    }
    void update()
    {
       coords.y += speed;
    }
};



class ParticleSystem
{
    std::vector<Particle*> particles;
    GLuint particleID;
public:
    ParticleSystem()
    {
        particleID = glCreateProgram();
        int fragment = loadShaders("shaders/vertex/vertexShader.h",GL_VERTEX_SHADER);
        int vertex = loadShaders("shaders/fragment/paintShader.h",GL_FRAGMENT_SHADER);
        glAttachShader(particleID,fragment);
        glAttachShader(particleID, vertex);
        glLinkProgram(particleID);
        glDeleteShader(fragment);
        glDeleteShader(vertex);
    }
    void addParticle(Particle* p)
    {
        particles.push_back(p);
    }
    void render(Sprite& s)
    {
        glUseProgram(particleID);
        int size = particles.size();
        float colors[size*3];
        std::vector<glm::vec3> pos;
        int j = 0;
        for (int i = 0; i < size; ++i)
        {
            particles[i]->update();
            glm::vec3 color = particles[i]->getColor();
            colors[j] = color.x;
            colors[j+1] = color.y;
            colors[j+2] = color.z;
            j+= 3;
            pos.push_back(glm::vec3(particles[i]->getCoords(),0));
        }
        GLuint VAO = s.getVAO();
        glBindVertexArray(VAO);
    GLuint colorBuffer;
        glGenBuffers(1,&colorBuffer);

        glBindBuffer(GL_ARRAY_BUFFER,colorBuffer);
        glBufferData(GL_ARRAY_BUFFER,size*3*sizeof(float), colors,GL_STATIC_DRAW);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);
        glVertexAttribDivisor(2,1);
        glEnableVertexAttribArray(2);
       s.renderInstanced(pos, 16,16);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);

    }
};

#endif // SPARKLES_H_INCLUDED
