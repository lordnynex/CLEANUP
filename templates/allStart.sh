#!/bin/sh
tmux new-session -s minecraft -d 'htop'

# BUNGEECORD ROOT
echo "Starting Core BungeeCord Services"
# start the bungeecord core server
tmux new-window -t minecraft:1 -n 'BungeeCord' 'cd bungeeCord; ./start.sh; bash'

# BUNGEECORD BACKENDS
{% for server in servers %}
# {{ servers[server].instanceDir }}
echo "Starting {{ servers[server].instanceDir }}"
tmux new-window -t minecraft -n '{{ servers[server].instanceDir }}' 'cd backend/{{ servers[server].instanceDir }}; ./start.sh; bash'

{% endfor %}
# LAUNCH COMPLETE
# attatch to the bungeecord session
echo "Startup complete, connecting to BungeeCord Root"
tmux select-window -t minecraft:1
tmux -2 attach-session -t minecraft

