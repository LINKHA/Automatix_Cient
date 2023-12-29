local moon = require("moon")
local core = require("asio.core")

local make_session = moon.make_session
local id = moon.id

local close = core.close
local accept = core.accept
local connect = core.connect
local read = core.read
local write = core.write
local udp = core.udp
local unpack_udp = core.unpack_udp

local flag_close = 2

local supported_tcp_protocol = {
    [moon.PTYPE_SOCKET_TCP] = "tcp",
    [moon.PTYPE_SOCKET_WS] = "ws",
    [moon.PTYPE_SOCKET_MOON] = "moon",
    tcp = moon.PTYPE_SOCKET_TCP,
    ws = moon.PTYPE_SOCKET_WS,
    moon = moon.PTYPE_SOCKET_MOON
}

---@class socket : asio
local socket = core

---@async
---@param listenfd integer
---@param serviceid? integer
function socket.accept(listenfd, serviceid)
    serviceid = serviceid or id
    local sessionid = make_session()
    if not accept(listenfd, sessionid, serviceid) then
        error("invalid accept param")
    end
    local fd, err = moon.wait(sessionid)
    if not fd then
        return nil, err
    end
    return fd
end

function socket.start(listenfd)
    accept(listenfd, 0, id)
end

---@async
---@param host string
---@param port integer
---@param protocol integer|string # "tcp", "ws", "moon" 
---@param timeout? integer # millseconds
function socket.connect(host, port, protocol, timeout)
    assert(supported_tcp_protocol[protocol], "not support")
    if type(protocol) == "string" then
        protocol = supported_tcp_protocol[protocol]
    end
    timeout = timeout or 0
    local sessionid = make_session()
    connect(host, port, protocol, sessionid, timeout)
    local fd, err = moon.wait(sessionid)
    if not fd then
        return nil, err
    end
    return fd
end

--- NOTE:  used only when protocol == moon.PTYPE_SOCKET_TCP
---@async
---@param delim string @read until reach the specified delim string from the socket
---@param maxcount? integer
---@overload fun(fd: integer, count: integer) @ read a specified number of bytes from the socket.
function socket.read(fd, delim, maxcount)
    local sessionid = make_session()
    read(fd, sessionid, delim, maxcount)
    return moon.wait(sessionid)
end

function socket.write_then_close(fd, data)
    write(fd, data, flag_close)
end

local socket_data_type = {
    connect = 1,
    accept = 2,
    message = 3,
    close = 4,
}

---@alias socket_event
---| 'connect'
---| 'accept'
---| 'message'
---| 'close'

--- PTYPE_SOCKET_MOON callbacks
local callbacks = {}

local _decode = moon.decode

moon.raw_dispatch(
    "moonsocket",
    function(msg)
        local fd, sdt = _decode(msg, "SR")
        local f = callbacks[sdt]
        if f then
            f(fd, msg)
        end
    end
)

--- NOTE: used only when protocol == moon.PTYPE_SOCKET_MOON
---@param name socket_event
---@param cb fun(fd:integer, msg:message_ptr)
function socket.on(name, cb)
    local n = socket_data_type[name]
    if n then
        callbacks[n] = cb
    else
        error("register unsupport socket data type " .. name)
    end
end

local udp_callbacks = {}

moon.raw_dispatch(
    "udp",
    function(msg)
        local fd, p, n = _decode(msg, "SC")
        local fn = udp_callbacks[fd]
        if not fn then
            moon.error("drop udp message from", fd)
            return
        end
        local from, str = unpack_udp(p, n)
        fn(str, from)
    end
)

---@param cb fun(data:string, endpoint:string)
---@param host? string @ bind host
---@param port? integer @ bind port
function socket.udp(cb, host, port)
    local fd = udp(host, port)
    udp_callbacks[fd] = cb
    return fd
end

function socket.close(fd)
    close(fd)
    udp_callbacks[fd] = nil
end

return socket
