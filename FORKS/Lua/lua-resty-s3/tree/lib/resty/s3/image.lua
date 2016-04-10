local magick = require "resty.magick"

local thumb
thumb = function(blob, size)
    local src, err, _ = magick.load_image_from_blob(blob)
    if not src then
        return nil, err 
    end
    
    if not size then
        return nil, "no thumbnail size"
    end 
   
    local src_w, src_h = src:get_width(), src:get_height() 
    local opts = magick.parse_size_str(size, src_w, src_h)
    if not opts then
        return nil, "invalid thumbnail size"
    end
    if opts.center_crop then
        src:resize_and_crop(opts.w, opts.h)
    elseif opts.crop_x then
        src:crop(opts.w, opts.h, opts.crop_x, opts.crop_y)
    else
        src:resize(opts.w, opts.h)
    end
    
    local dst_img = src:get_blob()
    local r, err = magick.get_exception(src.wand)
    src:destroy()
    if r ~= 0 then
        return nil, err
    else
        return dst_img
    end
end

local noise
noise = function(blob, passwd)
    --TODO
end

return {
    thumb = thumb,
    noise = noise
}
