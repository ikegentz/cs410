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
            this->process_pixel(this->pixel_array[row][col], this->pixel_array[row][col].ray, data->recurse_depth);
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
    ray.position = pixpt;
    ray.set_direction(shoot);
}

//        hit? where?  Av        Bv          Cv      model_index
std::tuple<bool, double, glm::vec3, glm::vec3, glm::vec3, int, Ray> Image::ray_cast_model(const Ray& ray)
{
    glm::vec3 retAv, retBv, retCv;
    int model_index = 0, i = 0;
    bool hit = false;
    double last_t = -1;
    Ray Rv;

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

            glm::vec3 Lv = ray.position;
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


                    glm::vec3 intersection_point = ray.position + (float)t*ray.get_direction();
                    // we hit something, so calculate reflecting ray
                    glm::vec3 E1 = retBv - retAv;
                    glm::vec3 E2 = retCv - retAv;
                    // calculate an initial surface normal, might get flipped down below
                    glm::vec3 N = glm::normalize(glm::cross(E1, E2));
                    // check if we need to flip
                    glm::vec3 toIntersection = glm::normalize(intersection_point - ray.position);
                    if(glm::dot(toIntersection, N) > 0)
                        N = -N;

                    glm::vec3 new_direc = 2*glm::dot(-ray.get_direction(), N) * N - (-ray.get_direction());
                    Rv.set_direction(new_direc);
                    Rv.position = intersection_point;
                }
            }
        }
        ++i;
    }

    if(hit)
        return std::make_tuple(true, last_t, retAv, retBv, retCv, model_index, Rv);
    else
        return std::make_tuple(false, -1, retAv, retBv, retCv, model_index, Rv);
}

//        hit?  where  sphere_index  pt
std::tuple<bool, double, int, glm::vec3, Ray> Image::ray_cast_sphere(const Ray& ray)
{
    // loop through all spheres
    float d = 0.0f;
    bool hit = false;
    int count = 0, sphere_index = 0;
    double last_t = -1;
    glm::vec3 pt;
    Ray Rv;

    for(Sphere sphere : data->spheres)
    {
        glm::vec3 C = sphere.position;
        glm::vec3 L = ray.position;
        glm::vec3 U = ray.get_direction();
        glm::vec3 T = C - L;

        float v = glm::dot(T, U);
        float csq = glm::dot(T, T);
        float disc = sphere.radius*sphere.radius - (csq - v*v);

        if(disc > 0)
        {
            d = sqrt(disc);
            float t = v-d;
            pt = L + U*t;
            float t_temp = glm::length(pt - L);

            if(t_temp < last_t || !hit)
            {
                last_t = t_temp;
                hit = true;
                sphere_index = count;

                glm::vec3 intersection_point = ray.position + (float)last_t*ray.get_direction();
                glm::vec3 N = glm::normalize(intersection_point - sphere.position);

                glm::vec3 new_direc = 2*glm::dot(-ray.get_direction(), N) * N - (-ray.get_direction());
                Rv.set_direction(new_direc);
                Rv.position = intersection_point;

               // goto skipCircle;
            }
        }
        ++count;
    }
    skipCircle:;
    return std::make_tuple(hit, last_t, sphere_index, pt, Rv);
}

void Image::process_pixel(Pixel &pixel, Ray& ray, int depth)
{
    if(depth < 0)
    {
        //std::cout << glm::to_string(pixel.rgba) << std::endl;
        return;
    }

    Material mat;
    Ray Rv;
    std::tuple<bool, double, glm::vec3, glm::vec3, glm::vec3, int, Ray> cast_results = ray_cast_model(ray);

    if(std::get<0>(cast_results) == true)
    {
        ray.last_t = std::get<1>(cast_results);
        ray.hit = true;
        ray.hit_sphere = false;
        ray.Av = std::get<2>(cast_results);
        ray.Bv = std::get<3>(cast_results);
        ray.Cv = std::get<4>(cast_results);
        mat = data->models[std::get<5>(cast_results)].material;

        Rv = std::get<6>(cast_results);
    }

    Sphere the_hit_sphere;
    glm::vec3 pt;
    std::tuple<bool, double, int, glm::vec3, Ray> cast_results_sphere = ray_cast_sphere(ray);

    if(std::get<0>(cast_results_sphere) == true)
    {
        ray.hit = true;
        ray.hit_sphere = true;
        ray.last_t = std::get<1>(cast_results_sphere);
        the_hit_sphere = data->spheres[std::get<2>(cast_results_sphere)];
        pt = std::get<3>(cast_results_sphere);
        Rv = std::get<4>(cast_results_sphere);
    }

    // for first loop, we want to set pixel EQUAL to color value
    if(depth == data->recurse_depth)
    {
        if (ray.hit)
        {
            if (ray.hit_sphere)
                pixel.rgba = this->color_me_sphere(pt, ray, the_hit_sphere);
            else
                pixel.rgba = this->color_me(pixel.ray.get_direction() * float(ray.last_t) + data->camera.eye, mat, ray);
        }
        else
        {
            pixel.rgba = glm::vec4(0.0, 0.0, 0.0, 1.0); // write black for background color
            depth = -1;
        }
    }
    // otherwise we are recursing and want to ADD to pixel color, the reflected surface color
    else
    {
        if(ray.hit)
        {
            if (ray.hit_sphere)
            {
              //  pixel.rgba += (float)pow(0.5, data->recurse_depth-depth)*this->color_me_sphere(pt, ray, the_hit_sphere);
              pixel.rgba += (float)0.5 * this->color_me_sphere(pt, ray, the_hit_sphere);
                //pixel.rgba = glm::vec4(ret.x*mat.kr.x * 1.0, ret.y*mat.kr.y * 1.0, ret.z*mat.kr.z * 1.0, 1);
            }
            else
            {
                //pixel.rgba += (float)0.1*this->color_me(pixel.ray.get_direction() * float(ray.last_t) + data->camera.eye, mat, ray);
            }
        }
        else
        {
           // pixel.rgba = glm::vec4(0.0, 0.0, 0.0, 1.0); // write black for background color
            depth = -1;
        }
    }
   process_pixel(pixel, Rv, depth-1);
}

glm::vec4 Image::color_me_sphere(glm::vec3 intersection_point, const Ray& ray, const Sphere& sphere)
{
    glm::vec3 ptos = intersection_point;
    glm::vec3 snrm = glm::normalize(ptos - sphere.position);
    Material mat = sphere.material;
    glm::vec3 color = glm::vec3(data->camera.ambient.x*mat.ka.x, data->camera.ambient.y*mat.ka.y, data->camera.ambient.z*mat.ka.z);

    for(LightSource light : data->light_sources)
    {
        glm::vec3 ptL = light.position;
        glm::vec3 emL = light.rgb_amount;
        glm::vec3 toL = glm::normalize(ptL - ptos);

        // check for sphere shadows
        if(ray.last_t >= 0.0001f)
        {
            for (Sphere sphere : data->spheres)
            {
                glm::vec3 C = sphere.position;
                glm::vec3 L = intersection_point;
                glm::vec3 U = glm::normalize(light.position - L);

                float v = glm::dot(C - L, U);
                float c = glm::length(C - L);
                float r = sphere.radius;
                float dsq = r * r - (c * c - v * v);

                if (dsq >= 0)
                {
                    float d = sqrt(dsq);
                    float t = v-d;

                    if(t > 0)
                        goto nextLight;
                }
            }
        }
        ////end shadow checks

        if(glm::dot(snrm, toL) > 0.0)
        {
            glm::mat3x3 kds = glm::mat3x3(glm::vec3(mat.kd.x, 0, 0), glm::vec3(0, mat.kd.y, 0), glm::vec3(0, 0, mat.kd.z));
            color += kds * emL * glm::dot(snrm, toL);
            glm::vec3 toC = glm::normalize(ray.position - ptos);
            glm::vec3 spR = (2* glm::dot(snrm, toL) * snrm) - toL;
            float CdR = glm::dot(toC, spR);

            if(CdR > 0.0f)
            {
                glm::mat3x3 kdsS = glm::mat3x3(glm::vec3(mat.ks.x, 0, 0), glm::vec3(0, mat.ks.y, 0), glm::vec3(0, 0, mat.ks.z));
                color += kdsS * emL * float(pow(CdR, mat.PHONG));
            }
        }

        nextLight:;
    }

    return glm::vec4(color.x, color.y, color.z, 1.0);
}

glm::vec4 Image::color_me(glm::vec3 intersection_point, Material &mat, const Ray& ray)
{
    glm::vec3 I = glm::vec3(data->camera.ambient.x*mat.ka.x, data->camera.ambient.y*mat.ka.y, data->camera.ambient.z*mat.ka.z); // get ambient as base light amount
    // 2 edges of the triangle (face)
    glm::vec3 E1 = ray.Bv - ray.Av;
    glm::vec3 E2 = ray.Cv - ray.Av;

    // calculate an initial surface normal, might get flipped down below
    glm::vec3 N = glm::normalize(glm::cross(E1, E2));

    // check if we need to flip
    glm::vec3 toIntersection = glm::normalize(intersection_point - ray.position);
    if(glm::dot(toIntersection, N) > 0)
        N = -N;

    glm::vec3 diffuse;
    glm::mat3x3 kds;

    for(LightSource light : data->light_sources)
    {
        glm::vec3 toL = glm::normalize(light.position - intersection_point);
        // check if we have the 'correct' surface normal, or need to flip it
        // dot product should be negative for 'correct' surface normal
        if(glm::dot(N, toL) > 0)
            N = -N;

        if(glm::dot(N, toL) > 0)
            continue;

        glm::vec3 shadow_intersec_point = ray.position + ray.get_direction() * float(ray.last_t);
        glm::vec3 Lray;
        glm::vec3 fixed_light_position;

        // get direction to light source
        if(light.infinity)
        {
            Lray = glm::normalize(light.position * 1000000.0f - intersection_point);
            fixed_light_position = light.position * 10000000.0f - shadow_intersec_point;
        }
        else
        {
            Lray = glm::normalize(light.position - intersection_point);
            fixed_light_position = light.position - shadow_intersec_point;
        }

        // check for sphere shadows
        if(ray.last_t >= 0.0001f)
        {
            for (Sphere sphere : data->spheres)
            {
                glm::vec3 C = sphere.position;
                glm::vec3 L = shadow_intersec_point;
                glm::vec3 U = glm::normalize(fixed_light_position - L);

                float v = glm::dot(C - L, U);
                float c = glm::length(C - L);
                float r = sphere.radius;
                float dsq = r * r - (c * c - v * v);

                if (dsq >= 0)
                {
                    float d = sqrt(dsq);
                    float t = v-d;

                    if(t > 0)
                        goto nextLight;
                }
            }
        }
        ////end shadow checks


        // Idiffuse = KdB(N dot L)
        kds = glm::mat3x3(glm::vec3(mat.kd.x, 0, 0), glm::vec3(0, mat.kd.y, 0), glm::vec3(0,0,mat.kd.z));
        diffuse = kds * light.rgb_amount * (glm::dot(N, -Lray)); // this is positive in the slides but.. this makes it work so idk :/
        I = I + diffuse;

        nextLight:;
    }

    return glm::vec4(I.x, I.y, I.z, 1.0);

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

