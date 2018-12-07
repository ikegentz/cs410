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
            // this->process_pixel(this->pixel_array[row][col], this->pixel_array[row][col].ray, data->recurse_depth);
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
        {
            return true;
        }
    }
    return false;
}

void Image::pt_illum(Ray ray, glm::vec3 N, const Material& mat, glm::vec3& accum, glm::vec3 refatt)
{
    glm::vec3 color; // set initial color to ambient light
    color.x = mat.ka.x;
    color.y = mat.ka.y;
    color.z = mat.ka.z;
    //std::cout << "AMB: " << glm::to_string(color) << std::endl;


    for (LightSource lt : data->light_sources)
    {
        glm::vec3 toL = glm::normalize(lt.position - ray.bestPt);

        float NdotL = glm::dot(N, toL);
        if(NdotL > 0.0f && !shadow(ray.bestPt, lt))
        {
          //  std::cout << "PRE: " << glm::to_string(color) << std::endl;

            color.x += mat.kd.x * lt.E.x * NdotL;
            color.y += mat.kd.y * lt.E.y * NdotL;
            color.z += mat.kd.z * lt.E.z * NdotL;

           // std::cout << "POST: " << glm::to_string(color) << std::endl << std::endl;

            //std::cout << "LIGHT: " << glm::to_string(lt.E) << std::endl;
            //std::cout << "NdotL: " << NdotL << std::endl;

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
            std::cout << "MADE IT HERE REFLECT!!!" << std::endl;
        }
        // REFRACTION
        if(level > 0 && mat.ko.x+mat.ko.y+mat.ko.z < 3.0f)
        {
            glm::vec3 thru = glm::vec3(0,0,0);
            bool test;
            glm::vec3 pos;
            glm::vec3 direc;
            std::tie(test, pos, direc) = ray.bestSphere.refract_exit(-1.0f*ray.get_direction(),ray.bestPt, mat.eta, ray.bestSphere);
            if(test)
            {
                glm::vec3 temp;
                temp.x = mat.kr.x * refatt.x;
                temp.y = mat.kr.y * refatt.y;
                temp.z = mat.kr.z * refatt.z;
                ray_trace(Ray(pos, direc), thru, temp, level-1); // TODO FINISH THIS -- CHECK MAT.ko??
                accum.x += refatt.x * (1.0 - mat.ko.x) * thru.x;
                accum.y += refatt.y * (1.0 - mat.ko.y) * thru.y;
                accum.z += refatt.z * (1.0 - mat.ko.z) * thru.z;
                std::cout << "MADE IT HERE REFRACT!!!" << std::endl;
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

