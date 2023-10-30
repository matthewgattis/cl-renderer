#define MAX_DISTANCE        (16.0f)
#define MIN_DELTA           (0.000001f)
#define MAX_REFLECTIONS     (8)

#ifndef M_PI
#define M_PI                (3.1415926535897932384626433832795f)
#endif

struct surface_feature
{
    int obj;
    int obj_shad;
    float3 p;
    float3 n;
    float orbit;
};

// ------------------------------------------------------------------------------------------------
float2 rand2n(float2 *seed)
{
    *seed += (float2)(-1.0f, 1.0f);
    // implementation based on: lumina.sourceforge.net/Tutorials/Noise.html
    float s = 0.0f;
    return (float2)(fract(sin(dot((*seed).xy, (float2)(12.9898f, 78.233f))) * 43758.5453f, &s),
        fract(cos(dot((*seed).xy, (float2)(4.898f, 7.23f))) * 23421.631f, &s));
}

// ------------------------------------------------------------------------------------------------
float3 ortho(float3 v)
{
    //  See : http://lolengine.net/blog/2013/09/21/picking-orthogonal-vector-combing-coconuts
    return fabs(v.x) > fabs(v.z) ? (float3)(-v.y, v.x, 0.0f) : (float3)(0.0f, -v.z, v.y);
}

// ------------------------------------------------------------------------------------------------
float3 getSampleBiased(float3 dir, float power, float2 *seed)
{
    dir = normalize(dir);
    float3 o1 = normalize(ortho(dir));
    float3 o2 = normalize(cross(dir, o1));
    float2 r = rand2n(seed);
    r.x = r.x * 2.0f * M_PI;
    r.y = pow(r.y, 1.0f / (power + 1.0f));
    float oneminus = sqrt(1.0f - r.y*r.y);
    return cos(r.x)*oneminus*o1 + sin(r.x)*oneminus*o2 + r.y*dir;
}

// ------------------------------------------------------------------------------------------------
float3 getSample(float3 dir, float2 *seed)
{
    return getSampleBiased(dir, 0.0f, seed); // <- unbiased!
}

// ------------------------------------------------------------------------------------------------
float3 getCosineWeightedSample(float3 dir, float2 *seed)
{
    return getSampleBiased(dir, 1.0f, seed);
}

// ------------------------------------------------------------------------------------------------
float3 getConeSample(float3 dir, float extent, float2 *seed)
{
    // Formula 34 in GI Compendium
    dir = normalize(dir);
    float3 o1 = normalize(ortho(dir));
    float3 o2 = normalize(cross(dir, o1));
    float2 r = rand2n(seed);
    r.x = r.x * 2.0f * M_PI;
    r.y = 1.0f - r.y*extent;
    float oneminus = sqrt(1.0f - r.y*r.y);
    return cos(r.x)*oneminus*o1 + sin(r.x)*oneminus*o2 + r.y*dir;
}

// ------------------------------------------------------------------------------------------------
float3 vRotateX(float3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return (float3)(p.x, c*p.y + s*p.z, -s*p.y + c*p.z);
}

// ------------------------------------------------------------------------------------------------
float3 vRotateY(float3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return (float3)(c*p.x - s*p.z, p.y, s*p.x + c*p.z);
}

// ------------------------------------------------------------------------------------------------
float3 vRotateZ(float3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return (float3)(c*p.x + s*p.y, -s*p.x + c*p.y, p.z);
}

// ------------------------------------------------------------------------------------------------
float sphere(float3 p, float r)
{
    return length(p) - r;
}

// ------------------------------------------------------------------------------------------------
#define DE2_SCALE   (1.5f)
#define DE2_OFFSET  (2.0f)
#define DE2_ITER    (48)
float DE2(float3 z, float *orbit)
{
    float r;
    float d;

    float min_dist = 1e9f;

    for (int n = 0; n < DE2_ITER; n++)
    {
        z = vRotateX(z, -M_PI * 0.25f);
        if (z.x + z.y < 0.0f) { z.xy = -z.yx; } // fold 1
        if (z.x + z.z < 0.0f) { z.xz = -z.zx; } // fold 2
        if (z.y + z.z < 0.0f) { z.zy = -z.yz; } // fold 3  
        //z.xyz = fabs(z.xyz);
        z = z * DE2_SCALE - DE2_OFFSET * (DE2_SCALE - 1.0f);

        d = length(z);
        min_dist = min(min_dist, d);
    }

    *orbit = min_dist;
    return d * pow(DE2_SCALE, -(float)DE2_ITER);
}

// ------------------------------------------------------------------------------------------------
#define DE3_ITER            (32)
#define DE3_SCALE           (2.0f)
#define DE3_MIN_RADIUS      (0.5f)
#define DE3_FIXED_RADIUS    (1.0f)
#define DE3_FOLDING_LIMIT   (1.0f)
void sphereFold(float3 *z, float *dz)
{
    float r2 = dot(*z, *z);

    if (r2 < DE3_MIN_RADIUS)
    { 
        const float temp = DE3_FIXED_RADIUS / DE3_MIN_RADIUS;
        *z *= temp;
        *dz *= temp;
    }
    else if (r2 < DE3_FIXED_RADIUS)
    { 
        float temp = DE3_FIXED_RADIUS / r2;
        *z *= temp;
        *dz *= temp;
    }
}

void boxFold(float3 *z)
{
    const float3 foldingLimit = (float3)(DE3_FOLDING_LIMIT, DE3_FOLDING_LIMIT, DE3_FOLDING_LIMIT);
    *z = clamp(*z, -foldingLimit, foldingLimit) * 2.0f - *z;
}

float DE3(float3 z, float *orbit)
{
    float3 offset = z;
    float dr = 1.0f;

    float min_dist = -1e9f;

    for (int n = 0; n < DE3_ITER; n++)
    {
        boxFold(&z);
        sphereFold(&z, &dr);

        z  = DE3_SCALE * z + offset;
        dr = dr * fabs(DE3_SCALE) + 1.0f;

        min_dist = max(min_dist, length(z));
    }

    *orbit = min_dist;

    float r = length(z);
    return r / fabs(dr);
}

// ------------------------------------------------------------------------------------------------
float getMap(float3 p, struct surface_feature *surf)
{
    float t;
    float tp;

    surf->obj = 1;
    t = DE3(p, &(surf->orbit));

    return t;
}

// ------------------------------------------------------------------------------------------------
float3 getNormal(float3 p)
{
    float h = MIN_DELTA;
    struct surface_feature surf;
    return normalize((float3)(
        getMap(p + (float3)(h, 0.0f, 0.0f), &surf) - getMap(p - (float3)(h, 0.0f, 0.0f), &surf),
        getMap(p + (float3)(0.0f, h, 0.0f), &surf) - getMap(p - (float3)(0.0f, h, 0.0f), &surf),
        getMap(p + (float3)(0.0f, 0.0f, h), &surf) - getMap(p - (float3)(0.0f, 0.0f, h), &surf)));
}

// ------------------------------------------------------------------------------------------------
float castRay(float3 o, float3 d, struct surface_feature *surf)
{
    float t = 0.0f;
    float dt;
    float3 p;

    p = o;

    for (int i = 0; i < 4096; i++)
    {
        dt = getMap(p, surf);
        t += dt;
        p = o + d * t;

        if (fabs(dt) <= MIN_DELTA)
        {
            surf->p = p;
            return t;
        }

        if (t >= MAX_DISTANCE)
            break;
    }

    surf->obj = 0;
    return 0.0f;
}

float3 getBackground(float3 d)
{
    //return (float3)(1.0f, 1.0f, 1.0f);
    return (float3)(0.8f, 0.9f, 1.0f);
    //return -d / 2.0f + 0.5f;
}

void focus(float a, float b, float3 *o, float3 *d, float2 *seed)
{
    float t = a * length(*d);
    float3 p = *o + a * *d;
    *d = getConeSample(*d, b, seed);
    *o = p - t * *d;
}

// ------------------------------------------------------------------------------------------------
float3 pal(float t, float3 a, float3 b, float3 c, float3 d )
{
    const float TWO_PI = 2.0f * M_PI;
    return a + b*cos(TWO_PI * (c * t + d));
}

// ------------------------------------------------------------------------------------------------
float4 drawScene(float2 p, float2 *seed)
{
    struct surface_feature surf[MAX_REFLECTIONS];
    struct surface_feature surf_shad;

    float3 origin = (float3)(-2.6f, -4.0f, 6.2f);
    float3 direction = (float3)(p.x, p.y, -2.0f);

    float3 o = origin;
    float3 d = direction;

    d = vRotateY(d, 0.4f);
    d = vRotateX(d, -0.6f);

    *seed = *seed + (float2)(8436.4156f, 7864.1684f);
    focus(0.12f, 0.00001f, &o, &d, seed);

    const float3 light_vec = normalize((float3)(2.0f, 1.0f, 3.0f));

    int j = 0;
    for (int i = 0; i < MAX_REFLECTIONS; i++)
    {
        j++;

        *seed = (*seed + (float2)(8436.4156f, 7864.1684f)) * ((float)j + 1.0f);
        rand2n(seed);

        castRay(o, d, &(surf[i]));

        if (surf[i].obj == 0)
            break;

        float3 n = getNormal(surf[i].p);
        surf[i].n = n;

        o = surf[i].p + 2.0f * MIN_DELTA * n;
        d = getSample(n, seed);

        castRay(o, getConeSample(light_vec, 0.0001f, seed), &surf_shad);
        surf[i].obj_shad = surf_shad.obj;
    }

    float3 comp = (float3)(0.0f, 0.0f, 0.0f);

    while (j > 0)
    {
        j--;

        switch (surf[j].obj)
        {
            case 0:
                comp = getBackground(d);
                break;
            case 1:
                {
                    float3 color = 0.8f * pal(
                        log(surf[j].orbit),
                        (float3)(0.5f, 0.5f, 0.5f),
                        (float3)(0.5f, 0.5f, 0.5f),
                        (float3)(1.0f, 0.7f, 0.4f),
                        (float3)(0.0f, 0.15f, 0.20f));
                    color = mix(color, (float3)(1.0f, 1.0f, 1.0f), 1.0f / 32.0f);

                    comp *= 0.6f * color;

                    if (surf[j].obj_shad == 0)
                    {
                        comp += 4.0f * (float3)(1.0f, 0.8f, 0.6f) * color * max(0.0f, dot(surf[j].n, light_vec));
                    }
                    break;
                }
            default:
                comp *= 0.0f;
                break;
        }
    }

    return (float4)(comp, 1.0f);
}

// ------------------------------------------------------------------------------------------------
__kernel void mainimage(
    __global float4 *tile,
    int tile_size,
    int width,
    int height)
{
    int gid_x = get_global_id(0);
    int gid_y = get_global_id(1);
    int gid_z = get_global_id(2);

    float2 resolution = (float2)(width, height);

    float adjusted_sample = gid_z + 1.0f;
    float2 graph = (float2)(gid_x, gid_y);
    float2 seed =
        (graph + (float2)(4118.4898f, 2304.2126f)) * adjusted_sample * 2763.8911f;
    float2 offset = fabs(rand2n(&seed));
    graph = 2.0f * (graph + offset) / resolution.y;
    graph -= (float2)(resolution.x / resolution.y, 1.0f);

    float4 final = drawScene(graph, &seed);
    final = pow(final, (float4)(1.0f / 2.2f));

    //return;

    int idx =
        (gid_x % tile_size) + (gid_y % tile_size) * tile_size;

    tile[idx] = mix(
        tile[idx],
        final,
        1.0f / adjusted_sample);
}

