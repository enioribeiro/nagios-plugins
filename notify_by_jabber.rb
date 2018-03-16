#!/usr/bin/ruby
require 'xmpp4r'
require 'xmpp4r/muc'


inputs = ARGV

if inputs.length <= 0
   puts "Argumento inválido"
exit
end


room    = inputs[0]
password= inputs[1]
msg     = inputs[2]

jid = Jabber::JID.new('EMAIL');
@jabber = Jabber::Client.new(jid)
@jabber.connect
@jabber.auth('PASSWORD')
@jabber.send(Jabber::Presence.new.set_show(:chat).set_status('Online!'));

@room = Jabber::MUC::MUCClient.new(@jabber)
@room.join(room + '/NICK_NAME', password)

m = Jabber::Message.new
m.body = msg
@room.send(m)
