# Rigid Body Motion

## The Intermediate Axis Theorem
Imagine being an astronaut aboard the ISS. You unscrew a bolt from the control panels and observe the following phenomenon. 

{% include youtubePlayer.html id="1x5UiwEEvpQ" %}

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



### Further Resources
- [Wikipedia Reference](https://en.wikipedia.org/wiki/Tennis_racket_theorem)
- [Terence Tao's intuitive explanation](https://mathoverflow.net/a/82020)
- [Veritasium's visualization Tao's explanation](https://www.youtube.com/watch?v=1VPfZ_XzisU)
- [Dzhanibekov Effect in Space](https://user-images.githubusercontent.com/47429091/145022696-35249db9-51d7-41f9-b4bb-36b7ce644d72.mp4)


### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [Basic writing and formatting syntax](https://docs.github.com/en/github/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax).
