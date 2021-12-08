# Rigid Body Motion

## The Intermediate Axis Theorem
Imagine being an astronaut aboard the ISS. You unscrew a bolt from the control panels and observe the following phenomenon. 

<video src="https://user-images.githubusercontent.com/47429091/145022696-35249db9-51d7-41f9-b4bb-36b7ce644d72.mp4" controls="controls" style="max-width: 730px;">
</video>

What you see seems to defy Newton's first law. How is the object rotating without any external force?
Does this happen with any object, or is there something special about the bolt?

### Defining Our Model
We first notice that the shape of the bolt allows three unique axes of rotation.

![bolt-axis-schematic](https://user-images.githubusercontent.com/47429091/145023831-84d359e2-497d-47cb-ac33-537d130645f2.png)

In the video, the bolt rotates around its intermediate z-axis and exhibits this flipping behavior. But what about the other axes? To begin our investigation, we first define the parameters of the object we will use to simulate the rotation. In classic computer graphics flavor, we use the Stanford bunny as our model.

![bunny-axes](https://user-images.githubusercontent.com/47429091/145128096-5ae962af-1cd4-4ab7-b772-772dba7ab1af.png)

Similarly to the bolt, the bunny has three unique axes of rotation because its shape is not radially symmetric on any plane. In particular, its moments of inertia are different about each axis, so we can encode its inertia along each axis by using a diagonal 3x3 matrix, which we will call **M<sub>model</sub>**.

![M-model](https://user-images.githubusercontent.com/47429091/145130163-8ebb0808-d7c7-47ba-9998-ff3215050269.png)

In particular, **M<sub>model</sub>** describes the moments of inertia along the eigenvectors of the model, which we assume to just be the coordinate axes. Hence,
_μ<sub>1</sub>_, _μ<sub>2</sub>_, and _μ<sub>3</sub>_ are proportional to the squares of the widths of the bunny in the x, y, and z axes, respectively.

With this relationship in mind, we define _μ<sub>1</sub> = 4.0_, _μ<sub>2</sub> = 1.0_, and _μ<sub>3</sub> = 2.0_, and our model matrix becomes

![M_bunny](https://user-images.githubusercontent.com/47429091/145131829-c7f7e400-51d9-490f-b9b8-c08ba7617f8b.png)

In particular, the matrix encodes the fact that the bunny is roughly twice as wide along the x-axis as the y-axis, and roughly 1.5 times as wide on the z-axis as the y-axis. Hence, in our model, the z-axis is the intermediate axis of rotation. 

### Defining Our Animation
Now that we have the parameters of our object defined, how do we define the animation to be physically accurate? Let's take a look at the physics of rotation.

Recall from freshman physics class that energy and momentum are conserved. In particular, this is true for angular kinetic energy (since the object is rotating but not moving) and angular momentum as well. Also, for simplicity's sake we'll assume the object does not deform under rotational motion. Now we have some guidelines for the animation to follow:

1. **M<sub>model</sub>** remains constant.
2. _E<sub>kinetic</sub>_ (angular kinetic energy) remains constant.
3. _L_ (angular momentum) remains constant.

However, since we plan to simulate the rotation in code, our animation will happen in discrete frames rather than continuously like in the real world. Hence, we use a specific algorithm to numerically approximate the conditions stated above. 

![Buss's algorithm](https://user-images.githubusercontent.com/47429091/145158305-80a04198-eb5b-432c-9d2b-d0b5d3a16292.png)

In particular, this algorithm computes the rotational matrix for our object at each time step such that kinetic energy and angular momentum are conserved.
Now we've already defined **M<sub>model</sub>**, but what about the initial _R_ and _ω_? How do we compute _Δt_?

Using OpenGL, we can use `glutGet(GLUT_ELAPSED_TIME)` and global variables to keep track of the elapsed time between animation frames. In particular, we can keep track of `prev_time` and `curr_time` so that `Δt = (curr_time - prev_time) / 1000.0f;`. Note the division to convert from milliseconds to seconds.

_R(t)_ is the rotation matrix that dictates the orientation of our bunny at each frame. The rotation matrix is dependent on time, since the bunny is continuously rotating at each frame, but for simplicity we define the initial rotation matrix as _R = I_ (identity matrix). At each frame, we update the rotation matrix _R(t)_ so the bunny changes its orientation.

_ω_ is the angular velocity of the bunny. Normally, the velocity vector points in the direction of motion, but the angular velocity vector points in the direction of the axis of rotation. The magnitude of _ω_ corresponds to how quickly the angle is changing, i.e. how fast the bunny is spinning around the axis of rotation. We can arbitrarily decide how the bunny is initially spinning, so let's try it out.

### Defining the Rotation

First, we'll define 

![w_x](https://user-images.githubusercontent.com/47429091/145176143-75a82342-3ae7-4f4c-b5d8-c999a2f7d9e9.png)

Hence, the axis of rotation is almost exactly the x-axis, with a slight perturbation in the z-axis direction. Now let's run our animation using Buss's method with this initial _ω_.

<video src="https://user-images.githubusercontent.com/47429091/145177457-6f87032d-0e37-4334-bbe2-e0ed21e174b3.mov" controls="controls" style="max-width: 730px;">
</video>

While the bunny wobbles slightly around the x-axis, we don't see the repeating change in orientation like in our space example above.

Now what about the y-axis? Let 

![w_y](https://user-images.githubusercontent.com/47429091/145177981-d52a7285-23ae-4469-8662-98680029a469.png)

<video src="https://user-images.githubusercontent.com/47429091/145181045-7505e1b4-4196-40cd-b956-c9b7cbf982c2.mov" controls="controls" style="max-width: 730px;">
</video>

Again, we see some perturbation around the y-axis, but the bunny still does flip around like the space bolt?

Now we try the intermediate z-axis. Let

![w_z](https://user-images.githubusercontent.com/47429091/145182930-a76b53da-bb8e-4452-b486-0c8da8b3bb4b.png)

<video src="https://user-images.githubusercontent.com/47429091/145183484-c105164b-ca63-4210-8096-8dcbc1fa5c5b.mov" controls="controls" style="max-width: 730px;">
</video>

Success! The bunny's orientation changes while in rotation around the z-axis. Much like how the bolt changes the direction it's facing, the bunny takes turns facing towards us and away from us.

Now we can be somewhat confident that the constraints of constant angular momentum and kinetic energy result in the instability of rotation around the intermediate axis, but what exactly is this relationship? One way we can visualize the relationship is by constructing **Poinsot's ellipsoids**.

### Poinsot's Ellipsoids

Recall that the animation obeys three important conservation laws.

![Poinsot](https://user-images.githubusercontent.com/47429091/145185875-50ad2885-cf30-4e7a-8f92-f5a6d8562014.png)

Since kinetic energy stays constant no matter the orientation of the object, we can calculate all points (x,y,z) such that 

![Kinetic Poinsot](https://user-images.githubusercontent.com/47429091/145187624-2a7f86ee-63ce-46df-8d24-cdaa660fbd42.png)

These points form an ellipsoid where each point on its surface represents a magnitude and direction of the angular velocity that would keep _E<sub>kinetic</sub>_ at the same constant value.

Similarly, angular momentum is conserved in world coordinates, and in model coordinates its magnitude is constant, so we can find the set of (x,y,z) such that 

![Momentum Poinsot](https://user-images.githubusercontent.com/47429091/145188250-7b467aae-a836-4e01-9894-30ceff42dd89.png)

These points another ellipsoid whose surface is the points representing a magnitude and direction of the angular velocity that keeps **|L|<sup>2</sup>** constant.

The exact calculations behind the dimensions of the ellipsoids can be found in the [writeup](https://cseweb.ucsd.edu/~alchern/teaching/cse167_fa21/project-rigidmotion.pdf).

Now notice that for both conditions to be true at the same time, the angular velocity must lie on the intersection of the two ellipsoids!

So why are the large and small x and y axes so stable compared to the z-axis? Let's take a look at the rotation around the x-axis.

<video src="https://user-images.githubusercontent.com/47429091/145192655-ac55beb7-5adb-4c06-8473-8c0c4d288d77.mov" controls="controls" style="max-width: 730px;">
</video>

Notice how the intersection of the two ellipsoids (called the **polhode**) is an incredibly small circular region near the axis of rotation, so the angular velocity, represented as the black dot, makes small circular rotations around the axis of rotation and barely changes position, which explains the stability of rotation around the x-axis.

What about the y-axis?

<video src="https://user-images.githubusercontent.com/47429091/145193314-1030b7cc-2472-421e-8e06-2f5d0707d4da.mov" controls="controls" style="max-width: 730px;">
</video>

Again, the polhode is a small elliptical region around the axis of rotation, so the position of angular velocity barely changes and the bunny wobbles steadily around the y-axis.

Now let's take a look at the rotation around the intermediate z-axis.

<video src="https://user-images.githubusercontent.com/47429091/145190252-21914044-d15a-4f2d-930d-c0558d364c2f.mov" controls="controls" style="max-width: 730px;">
</video>

Notice the intersection of the two ellipsoids is considerably more complicated, and as a result the angular velocity traces out a more complicated shape compared to the small circles it was making earlier. We can now quantitatively see why the direction of rotation changes so wildly compared to the other axes.

Below are a few photos and videos detailing some other interesting aspects of the ellipsoids and orientations. Notice the ellipsoids depend on the moments of inertia of the objects and how **M<sub>model</sub>** directly influences the type of motion we observe around the axes.

Spefically, the cube has moments of inertia that are exactly the same along all coordinate axes, so there is no unstable intermediate axis, unlike the teapot and bunny.

<video src="https://user-images.githubusercontent.com/47429091/145194000-c1b18a63-a902-45a3-9c40-39e6650b8753.mov" controls="controls" style="max-width: 730px;">
</video>

### Further Resources
- [Tennis Racket Theorem on Wikipedia](https://en.wikipedia.org/wiki/Tennis_racket_theorem)
- [Terence Tao's intuitive explanation](https://mathoverflow.net/a/82020)
- [Veritasium's visualization Tao's explanation](https://www.youtube.com/watch?v=1VPfZ_XzisU)
- [Poinsot's ellipsoid on Wikipedia](https://en.wikipedia.org/wiki/Poinsot%27s_ellipsoid)
