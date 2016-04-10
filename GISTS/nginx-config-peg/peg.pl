############
# Grammar  #
############
my $grammar = do {
        qr@
        <nocontext:>
        #<debug:on>

        <file>

        <rule: file> (<[topblock]> <[comment]>*)+ $

        <rule: topblock>
                <server> | <upstream>

        <rule: upstream>
                ( (upstream) | <[comment]>+ (upstream) ) <name=word> <block>

        <rule: server>
                ( (server) | <[comment]>+ (server) ) <block>

        <rule: block>
                \{ <[line]>* ** (;) <minimize:> \}

        <rule: line>
                #<debug:step>
                (
                          <comment>     <type='comment'>
                        | <rewrite>     <type='rewrite'>
                        | <if>                  <type='if'>
                        | <location>    <type='location'>
                        # | <server>            <type='server'>
                        | <directive>   <type='directive'>
                )
                # <debug:off>

        <rule: comment>
                \# ([^\n]*) (?:\n)

        <rule: directive>
                <command=word>  <[arg]>* ** <.ws> (;)([ \t]*)<comment>?

        <rule: if>
                ((if) | <[comment]>+ (if)) \( <condition> \) <block>

        <rule: location>
                ((location) | <[comment]>+ (location))  <op=cop>? <where=locarg> <block>

        <rule: rewrite>
                (rewrite) (.+?) \n

        <rule: condition>       (<[opd]> (<[cop]> <[opd]>)?)+

        <rule: opd>             (\!? \-? \$? \w+)

        <rule: cop>             \|\| | \&\& | != | ==? | <<? | >>? | =~ | \+ | - | ~

        <rule: locarg>  [^{\s]+?

        <token: arg>    [a-zA-Z0-9_\$/\.:+*\\^(){}\[\]=\'\"-]+

        <rule: word>    \$?\w+
        @xs;
};
