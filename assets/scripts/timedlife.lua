require("assets/scripts/component")

_entity = 0
start = function(entity)
    _entity = entity
    lifetime = 1
end

update = function(dt)
    lifetime = lifetime - dt
    if lifetime <= 0 then
        Destroy(_entity)
    end
end
