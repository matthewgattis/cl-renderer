#pragma once

#include <memory>

#include "image.hpp"

class ImageMap : public Image
{
public:
    ImageMap(int width, int height, int tile_size);

    ImageMap(const Image& other, int tile_size);

    inline int tile_size() const;

    inline int tile_x_count() const;

    inline int tile_y_count() const;

public:
    inline virtual int width() const override;

    inline virtual int height() const override;

    virtual std::vector<float> pixels() const override;

private:
    int width_;
    int height_;
    int tile_size_;

    std::vector<std::shared_ptr<Image>> tiles_;
};

