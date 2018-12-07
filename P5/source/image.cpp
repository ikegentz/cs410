//
// Created by ikegentz on 10/11/18.
//

#include "image.h"

Image::Image()
{
    this->bounds = glm::vec4(0,0,0,1);
    this->res = glm::vec2(16, 16);
}

Image::Image(RaytracerData* data_)
{
    this->data = data_;
    this->bounds = glm::vec4(data->camera.bounds);
    this->res = glm::vec2(data->camera.res);
}

void Image::render_image()
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

    glm::vec3 wv = data->camera.eye - data->camera.look;
    wv = glm::normalize(wv);
    glm::vec3 uv = glm::cross(data->camera.up, wv);
    uv = glm::normalize(uv);
    glm::vec3 vv = glm::cross(wv, uv);
    vv = glm::normalize(vv);

    //we will be looping through rows, so go through vertically
    for(unsigned row = 0; row < this->res.x; ++row)
    {
        for(unsigned col = 0; col < this->res.y; ++col)
        {
            // set the ray's position and direction for this pixel
            this->pixelPt(row, col, -data->camera.d, data->camera.eye, wv, uv, vv);
            // cast this ray to see if it hits anything
            glm::vec3 rgb = glm::vec3(0,0,0);
            ray_trace(this->pixel_array[row][col].ray, rgb, glm::vec3(1.0f, 1.0f, 1.0f), data->recurse_depth);
            this->pixel_array[row][col].rgba = glm::vec4(rgb.x, rgb.y, rgb.z, 1.0f);
        }
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
    ray.L = pixpt;
    ray.set_direction(shoot);
}

bool Image::ray_find(Ray& ray)
{
    for (Sphere s : data->spheres)
        ray.sphere_test(s);

    return ray.hit;
}

bool Image::shadow(glm::vec3 pt, LightSource lt)
{
    glm::vec3 L = lt.position - pt;
    Ray ray = Ray(pt, L);
    float dtl = glm::dot(L, ray.get_direction());
    for (Sphere s : data->spheres)
    {
        if(ray.sphere_test(s) && ray.best_t < dtl)
            return true;
    }
    return false;
}

void Image::pt_illum(Ray ray, glm::vec3 N, const Material& mat, glm::vec3& accum, glm::vec3 refatt)
{
    glm::vec3 color = glm::vec3(0.2f, 0.2f, 0.2f); // set initial color to ambient light
    color.x *= mat.ka.x;
    color.y *= mat.ka.y;
    color.z *= mat.ka.z;

    for (LightSource lt : data->light_sources)
    {
        glm::vec3 toL = glm::normalize(lt.position - ray.bestPt);
        float NdotL = glm::dot(N, toL);
        if(NdotL > 0.0f && !shadow(ray.bestPt, lt))
        {
            color.x += mat.kd.x * lt.E.x * NdotL;
            color.y += mat.kd.y * lt.E.y * NdotL;
            color.z += mat.kd.z * lt.E.z * NdotL;

            glm::vec3 toC = glm::normalize(ray.L - ray.bestPt);
            glm::vec3 spR = glm::normalize((2 * NdotL * N) - toL);
            float CdR = glm::dot(toC, spR);

            if(CdR > 0.0f)
            {
                color.x += (mat.ks.x * lt.E.x) * pow(glm::dot(toC, spR), Sphere::PHONG); // Sphere PHONG constant
                color.y += (mat.ks.y * lt.E.y) * pow(glm::dot(toC, spR), Sphere::PHONG); // Sphere PHONG constant
                color.z += (mat.ks.z * lt.E.z) * pow(glm::dot(toC, spR), Sphere::PHONG); // Sphere PHONG constant
            }
        }
    }
    accum.x += refatt.x * mat.ko.x * color.x;
    accum.y += refatt.y * mat.ko.y * color.y;
    accum.z += refatt.z * mat.ko.z * color.z;
}

//        hit? where?  Av        Bv          Cv      model_index
std::tuple<bool, double, glm::vec3, glm::vec3, glm::vec3, int> Image::ray_cast_model(const Ray& ray)
{
    glm::vec3 retAv, retBv, retCv;
    int model_index = 0, i = 0;
    bool hit = false;
    double last_t = -1;

    for(Model m : data->models)
    {
        // get vertices so we can use face indices
        std::vector <Vertex> verts = m.obj.vertices;
        // get each face in the object
        for (unsigned face_index = 0; face_index < m.obj.faces.size(); ++face_index)
        {
            Face f = m.obj.faces[face_index];
            Vertex A = verts[f.v1 - 1];
            Vertex B = verts[f.v2 - 1];
            Vertex C = verts[f.v3 - 1];

            glm::vec3 Av = glm::vec3(A.x, A.y, A.z);
            glm::vec3 Bv = glm::vec3(B.x, B.y, B.z);
            glm::vec3 Cv = glm::vec3(C.x, C.y, C.z);

            glm::vec3 Lv = ray.L;
            glm::vec3 Dv = ray.get_direction();

            glm::mat3x3 M = glm::mat3x3(Av - Bv, Av - Cv, Dv);
            glm::mat3x3 M1 = glm::mat3x3(Av - Lv, Av - Cv, Dv);
            glm::mat3x3 M2 = glm::mat3x3(Av - Bv, Av - Lv, Dv);
            glm::mat3x3 M3 = glm::mat3x3(Av - Bv, Av - Cv, Av - Lv);

            double detM = glm::determinant(M);
            double beta = glm::determinant(M1) / detM;
            double gamma = glm::determinant(M2) / detM;
            double t = glm::determinant(M3) / detM;


            if (beta >= 0.0 && gamma >= 0.0 && beta + gamma <= 1.0 && t > 0.0)
            {
                if (t < last_t || !hit)
                {
                    last_t = t;
                    hit = true;
                    retAv = Av;
                    retBv = Bv;
                    retCv = Cv;
                    model_index = i;
                }
            }
        }
        ++i;
    }

    if(hit)
        return std::make_tuple(true, last_t, retAv, retBv, retCv, model_index);
    else
        return std::make_tuple(false, -1, retAv, retBv, retCv, model_index);
}

glm::vec3 Image::ray_trace(Ray ray, glm::vec3& accum, glm::vec3 refatt, int level)
{
    if(ray_find(ray))
    {
        glm::vec3 N = glm::normalize(ray.bestPt - ray.bestSphere.C);
        Material mat = ray.bestSphere.material;
        pt_illum(ray, N, mat, accum, refatt);
        // REFLECTION
        if(level > 0)
        {
            glm::vec3 flec = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 Univ = (-1.0f * ray.get_direction());
            glm::vec3 refR = glm::normalize((2*glm::dot(N, Univ) * N) - Univ);
            ray_trace(Ray(ray.bestPt, refR), flec, mat.kr * refatt, (level-1));
            accum.x += refatt.x * mat.ko.x * flec.x;
            accum.y += refatt.y * mat.ko.y * flec.y;
            accum.z += refatt.z * mat.ko.z * flec.z;
        }
        // REFRACTION
        if(level > 0 && mat.ko.x+mat.ko.y+mat.ko.z < 3.0f)
        {
            glm::vec3 thru = glm::vec3(0.0,0.0,0.0);
            glm::vec3 point;
            glm::vec3 direction;
            std::tie(point, direction) = ray.bestSphere.refract_exit(-1.0f*ray.get_direction(), ray.bestPt, mat.eta, 1.0f);
            Ray fraR(point, direction);
            if(glm::length(direction) > 0.01f) {
                glm::vec3 tempFatt;
                tempFatt.x = mat.kr.x * refatt.x;
                tempFatt.y = mat.kr.y * refatt.y;
                tempFatt.z = mat.kr.z * refatt.z;
                ray_trace(fraR, thru, tempFatt, level-1);

                accum.x += refatt.x * (1.0f - mat.ko.x) * thru.x;
                accum.y += refatt.y * (1.0f - mat.ko.y) * thru.y;
                accum.z += refatt.z * (1.0f - mat.ko.z) * thru.z;
            }
        }
    }
    return accum;
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

