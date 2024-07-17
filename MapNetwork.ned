network MapNetwork
{
    parameters:
        @display("bgb=449,300;bgi=background/terrain,s");
        double playgroundLatitude;
        double playgroundLongitude;
        double playgroundWidth @unit("m") = default(300m);
        double playgroundHeight @unit("m") = default(300m);

    submodules:
        osgEarthScene: OsgEarthScene {
            @display("p=74,31");
        }
        tx: Transmitter {
            @display("p=243,150");
        }
        rx: Receiver {
            @display("p=74,150");
        }
        rayTracer: RayTracingModule {
            @display("p=167,211");
        }

    connections:
        tx.out --> rx.in;

}
