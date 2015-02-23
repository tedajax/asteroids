require("assets/scripts/component")

_entity = 0
_lifetime = 5

function set_time(time)
    _lifetime = time
end

start = function(entity)
    _entity = entity
end

update = function(dt)
    _lifetime = _lifetime - dt
    if _lifetime <= 0 then
        Destroy(_entity)
    end
end
