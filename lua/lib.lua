-- values 0..360, 0..255, 0..510
function hsl2rgb(hue, saturation, lightness)
     hue = hue %360
     local tmp = lightness - 255
     local chroma = (255 - (tmp < 0 and -tmp or tmp)) * saturation / 255
     local h2 = (hue % 120) - 60
     local x = chroma * (60 - (h2 < 0 and -h2 or h2)) / 60
     local r = 0; g = 0; b = 0
     if hue >= 0 and hue < 60 then
          r = chroma
          g = x
     elseif hue < 120 then
          r = x
          g = chroma
     elseif hue < 180 then
          g = chroma
          b = x
     elseif hue < 240 then
          g = x
          b = chroma
     elseif hue < 300 then
          r = x
          b = chroma
     elseif hue < 360 then
          r = chroma
          b = x
     end
     m = (lightness - chroma) / 2
     return r + m, g + m, b + m
end
