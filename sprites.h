#ifndef SPRITES_H_INCLUDED
#define SPRITES_H_INCLUDED

int program;

int loadShaders(const GLchar*source, GLenum shaderType )
{
    std::ifstream input;
    input.open(source);
    int shader = -1;
    if (input.is_open())
    {


    std::stringstream stream;
    stream << input.rdbuf();
     shader = glCreateShader(shaderType);
     std::string str = stream.str();
    const char* code = str.c_str();
    glShaderSource(shader,1, &code, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Error loading shader " << "source: " << infoLog << std::endl;
    }
    }
    return shader;

}

class Point
{
public:

    double x, y;
};
class Sprite
{
    int width = 0, height = 0;
    int screenWidth = 0, screenHeight = 0;
    unsigned int texture = -1;
    float values[16] = {
    -1, 1, 0, 0,
    1, 1, 1, 0,
    -1, -1, 0, 1,
    1, -1, 1, 1

    };
    int indices[6] = {
    0,1,3,
    0,2,3


    };
    unsigned int VBO, VAO;
    void load(std::string source,bool transparent)
    {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1,&VBO);

        glBindVertexArray(VAO);


        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D,texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int channels;

        unsigned char* data = stbi_load(source.c_str(),&width, &height, &channels, 0);

        int rgb = GL_RGB*!transparent + GL_RGBA*transparent;
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, rgb,width, height, 0, rgb, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Error loading texture: " << source << std::endl;
        }
        stbi_image_free(data);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
public:
    Sprite(int stanWidth, int stanHeight, std::string source, bool transparent)
    {
        init(stanWidth,stanHeight,source,transparent);
    }
    Sprite()
    {

    }
    void init(int stanWidth, int stanHeight, std::string source, bool transparent)
    {
        load(source, transparent);
        screenWidth = stanWidth;
        screenHeight = stanHeight;
    }
    void loadBuffers()
    {
               //glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(values), values, GL_STATIC_DRAW);
            glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
                    glEnableVertexAttribArray(0);


        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      //  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    }
    void render(double x, double y, int w, int h, double radians)
    {
        double stanW = (double)w/width;
        double stanH = (double)h/height;
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x/(screenWidth/2.0) - (1-stanW),-(y/(screenHeight/2.0) - (1-stanH)),0));
        trans = glm::rotate(trans, (float)radians, glm::vec3(0,0,1));
        trans = glm::scale(trans, glm::vec3(stanW,stanH,1));

        glUniformMatrix4fv(glGetUniformLocation(program,"transform"),1,GL_FALSE,glm::value_ptr(trans));
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        loadBuffers();
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,&indices);
       //glDrawArrays(GL_TRIANGLES,0,6);

       glBindVertexArray(0);
    }
    void renderInstanced(const std::vector<glm::vec3>& points, int w, int h) //renders the same sprite at multiple locations
    {
        if (texture != -1)
        {
      //  std::cout << length << std::endl;
        int size = points.size();
        glm::mat4 matrices[size];
        glm::vec3 scale((float)w/screenWidth,(float)h/screenHeight,1);
     //   std::cout << scale.x<< scale.y << std::endl;
        glm::vec3 adjust(-(1-scale.x), 1- (scale.y),0);
        //std::cout << adjust.x << " " << adjust.y << std::endl;
        for (int i = 0; i < size; i ++)
        {
            glm::vec3 current = *(points.begin() + i);
            current.x = current.x/(screenWidth/2);
            current.y = -current.y/(screenHeight/2);
         //   std::cout << current.x << " " << current.y << std::endl;
            matrices[i] = glm::mat4(1.0f);
            matrices[i] = glm::translate(matrices[i],current);
            matrices[i] = glm::translate(matrices[i], adjust);
            matrices[i] = glm::scale(matrices[i],scale);
        }
        glBindVertexArray(VAO);
        unsigned int transform;
        glGenBuffers(1,&transform);
        glBindBuffer(GL_ARRAY_BUFFER,transform);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4)*size,matrices,GL_STATIC_DRAW);
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    loadBuffers();
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindTexture(GL_TEXTURE_2D,texture);
    glDrawElementsInstanced(GL_TRIANGLES,6,GL_UNSIGNED_INT,&indices,size);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        else
        {
            std::cout << "Haven't loaded a texture yet!" << std::endl;
        }
    }
    glm::mat4 getMat(double x, double y, int w, int h, double radians)
    {
        double stanW = (double)w/width;
        double stanH = (double)h/height;
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x/(screenWidth/2.0) - (1-stanW),-(y/(screenHeight/2.0) - (1-stanH)),0));
        trans = glm::rotate(trans, (float)radians, glm::vec3(0,0,1));
        trans = glm::scale(trans, glm::vec3(stanW,stanH,1));
        return trans;
    }
    int getVAO()
    {
        return VAO;
    }
};

#endif // SPRITES_H_INCLUDED
