### RDPscan - Remote desktop protocol password verfication

No external library dependencies, yay ;-P

### Installation

<pre>
git clone https://github.com/CaledoniaProject/rdpscan.git && make -C rdpscan/src
</pre>

### Usage

rdpscan accepts 4 parameters as stated below:

<pre>
 rdpscan host username credential [optional domain name]
</pre>

An example:

<pre>
 %> rdpscan 192.168.56.103 admin admin
 SUCCESSFUL LOGIN - 192.168.56.103 - (null)\admin : admin
</pre>

