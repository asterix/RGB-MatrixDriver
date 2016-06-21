# RGB Matrix Hat

I've designed and shared a small PCB which will let you convert Raspberry Pi's 3.3V GPIO outputs to a TTL compatible 5V levels that are needed for RGB LED matrices.
You'll need to populate this PCB with ```74HCT245``` with ```20-SOIC package``` and jumpers.

Top:

![PCBtop](../images/hat-top.png?raw=true)

Bottom:

![PCBbot](../images/hat-bottom.png?raw=true)


Note: This is revision 2 of this hat. It has been altered and re-verified for 16-IDC header compatibility.


The full design is available under ```RGBMatrixHat``` directory. This is done in ```Eagle```.

The first jumper is ```J1 = 12x2``` which is minimal and snaps straight into first ```12x2 pins``` of your Raspberry Pi. The second ```J2 = 8x2``` is an IDC header.
Just solder those in and you're good to go.

## Soldered Hat

Here are some photos of the soldered hat (Rev 2) to get a sense of header orientation.


Soldered Top:

![Stop](../images/soldered_top.png?raw=true)


Soldered Bottom:

![SBot](../images/soldered_bottom.png?raw=true)


Connected to Raspberry Pi:

![SPluggedIn](../images/soldered_in.png?raw=true)


## OSH Park share

Find this design shared on [OSH Park](https://oshpark.com/shared_projects/pfg9EhMr) or use the permalink ```https://oshpark.com/shared_projects/pfg9EhMr```.

Have fun!
