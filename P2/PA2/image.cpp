//
// Created by ikegentz on 10/11/18.
//

#include "image.h"

Image::Image()
{
    this->bounds = glm::vec4(0,0,0,1);
    this->res = glm::vec2(16, 16);
}

Image::Image(const glm::vec4 &bounds, const glm::vec2 &res)
{
    this->bounds = glm::vec4(bounds);
    this->res = glm::vec2(res);
}

void Image::render_image(const Camera &camera, std::vector<Model> &models, std::vector<LightSource> &lights)
{
    // create y rows of Pixels with x columns, default Pixel value
    this->pixel_array.resize(this->res.x, std::vector<Pixel>(this->res.y, Pixel()));
    std::cout << "CREATED IMAGE ARRAY OF " << this->res.x << " ROWS x " << this->res.y << " COLS" << std::endl;
    std::cout << "SHOOTING PIXEL RAYS INTO SCENE..." << std::endl;

    double left = this->bounds.x;
    double bottom = this->bounds.y;
    double right = this->bounds.z;
    double top = this->bounds.w;

    std::cout << "left: " << left << ", bottom: " << bottom << ", right: " << right << ", top: " << top<< std::endl;

    glm::vec3 wv = camera.eye - camera.look;
    wv = glm::normalize(wv);
    glm::vec3 uv = glm::cross(camera.up, wv);
    uv = glm::normalize(uv);
    glm::vec3 vv = glm::cross(wv, uv);
    vv = glm::normalize(vv);

    //we will be looping through rows, so go through vertically
    for(unsigned row = 0; row < this->res.x; ++row)
    {
        for(unsigned col = 0; col < this->res.y; ++col)
        {
            // set the ray's position and direction for this pixel
            this->pixelPt(row, col, -camera.d, camera.eye, wv, uv, vv);
            // cast this ray to see if it hits anything
            this->ray_cast(this->pixel_array[row][col], models, lights, camera);

            //print ASCII image
//            if(this->pixel_array[row][col].hit)
//                std::cout << " # ";
//            else
//                std::cout << " ' ";
        }
        // used for printing ASCII picture
        //std::cout << std::endl;
    }

}

void Image::pixelPt(const unsigned i, const unsigned j, const double near,
        const glm::vec3 &eye, const glm::vec3 &wv, const glm::vec3 &uv, const glm::vec3 &vv)
{
    Ray &ray = this->pixel_array[i][j].ray;
    float width = res.x;
    float height = res.y;
    double left = this->bounds.x;
    double bottom = this->bounds.y;
    double right = this->bounds.z;
    double top = this->bounds.w;

    double px = i/(width-1)*(right-left)+left;
    double py = j/(height-1)*(bottom-top) + top;

    glm::vec3 pixpt = eye + (wv * float(near)) + (uv * float(px)) + (vv * float(py)); // the ray's position
    glm::vec3 shoot = pixpt - eye;
    glm::normalize(shoot); // the ray's direction
    ray.position = pixpt;
    ray.set_direction(shoot);
}

void Image::ray_cast(Pixel &pixel, std::vector<Model> &models, std::vector<LightSource> &lights, const Camera &camera)
{
    // loop through all faces in the world (all models)
    Material &mat = models[0].material;
    glm::vec3 Av, Bv, Cv;
    for(Model m : models)
    {
        // get vertices so we can use face indices
        std::vector<Vertex> verts = m.obj.vertices;
        // get each face in the object
        for(unsigned face_index = 0; face_index < m.obj.faces.size(); ++face_index)
        {
            Face f = m.obj.faces[face_index];
            Vertex A = verts[f.v1-1];
            Vertex B = verts[f.v2-1];
            Vertex C = verts[f.v3-1];

            Av = glm::vec3(A.x, A.y, A.z);
            Bv = glm::vec3(B.x, B.y, B.z);
            Cv = glm::vec3(C.x, C.y, C.z);

            glm::vec3 Lv = pixel.ray.position;
            glm::vec3 Dv = pixel.ray.get_direction();

            glm::mat3x3 M = glm::mat3x3(Av-Bv, Av-Cv, Dv);
            glm::mat3x3 M1 = glm::mat3x3(Av-Lv, Av-Cv, Dv);
            glm::mat3x3 M2 = glm::mat3x3(Av-Bv, Av-Lv, Dv);
            glm::mat3x3 M3 = glm::mat3x3(Av-Bv, Av-Cv,Av-Lv);

            double detM = glm::determinant(M);
            double beta = glm::determinant(M1) / detM;
            double gamma = glm::determinant(M2) / detM;
            double t = glm::determinant(M3) / detM;

            if(beta >= 0.0 && gamma >= 0.0 && beta+gamma <= 1.0 && t > 0.0)
            {
                if(t < pixel.last_t || !pixel.hit)
                {
                    pixel.last_t = t;
                    pixel.face_index = face_index;
                    pixel.hit = true;
                    mat = m.material;
                }
            }
        }
    }
    if(pixel.hit)
    {
        pixel.rgba = this->color_me(/*pixel.ray.get_direction() * float(pixel.last_t) + pixel.ray.position*/ glm::vec3(0,0,0), mat, lights, camera.ambient,
                                    Av, Bv, Cv);
    }
    else
    {
        pixel.rgba = glm::vec4(0.0, 0.0, 0.0, 1.0); // write black for background color
    }
}

glm::vec4 Image::color_me(glm::vec3 intersection_point, Material &mat, std::vector<LightSource> &lights, glm::vec3 ambient,
        const glm::vec3 &Av, const glm::vec3 &Bv, const glm::vec3 &Cv)
{
    glm::vec3 I = glm::vec3(ambient.x * mat.ka.x, ambient.y * mat.ka.y, ambient.z * mat.ka.z); // get ambient as base light amount
    // 2 edges of the triangle (face)
    glm::vec3 E1 = Bv - Av;
    glm::vec3 E2 = Cv - Av;
    // calculate surface normal
    glm::vec3 N = glm::normalize(glm::cross(E1, E2));

//    for(LightSource light : lights)
//    {
//        glm::vec3 L;
//        // get direction to light source
//        if(light.infinity)
//        {
//            L = glm::normalize(light.position * 1000000.0f - intersection_point);
//        }
//        else
//        {
//            L = glm::normalize(light.position - intersection_point);
//        }
//        // Idiffuse = KdB(N dot L)
//        glm::vec3 diffuse = glm::vec3(mat.kd.x * light.rgb_amount.x, mat.kd.y * light.rgb_amount.y, mat.kd.z * light.rgb_amount.z) * glm::dot(N, L);
//        I = I + diffuse;
//    }

    return glm::vec4(I.x + mat.kd.x, I.y + mat.kd.y, I.z + mat.kd.z, 1.0);
}

void Image::write_image(const char* filename) const
{
    // We don't HAVE to do image write here but for simplicity write image after entire raycast is done
    // for speed though once we confirm everything works, we should write image as we go through the initial raycast
    // for each pixel
    std::ofstream outfile;
    outfile.open(filename);

    outfile << "P3" << std::endl;
    outfile << this->res.y << " " << this->res.x << " 255" << std::endl;

    //we will be looping through rows, so go through vertically
    for(unsigned row = 0; row < this->res.y; ++row)
    {
        for (unsigned col = 0; col < this->res.x; ++col)
        {
            glm::vec4 rgba = this->pixel_array[col][row].rgba;
            outfile << this->bound_rgb(rgba.r) << " ";
            outfile << this->bound_rgb(rgba.g) << " ";
            outfile << this->bound_rgb(rgba.b) << " ";
        }
        outfile << std::endl;
    }

    outfile.close();
}

unsigned Image::bound_rgb(double in_color) const
{
    int color_int = int(in_color * 255);
    if(color_int > 255)
        color_int = 255;
    if(color_int < 0)
        color_int = 0;

    return unsigned(color_int);
}

