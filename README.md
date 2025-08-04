<h1 id="zenoa-engine-c">Zenoa Engine (C++)</h1>

<p>Zenoa is a real-time 2D rigid-body physics engine built in modern C++17. It was developed as an educational project at age 17, with a focus on clarity, determinism, and stable simulation under simple physical assumptions.</p>

<p>The engine supports convex polygon and circle bodies, impulse-based collision resolution, and visual debugging through SFML. While compact in scope, it demonstrates working solutions to core physics simulation problems like friction, stacking, and energy conservation.</p>

<hr>

<h2 id="contents">Contents</h2>

<ul>
  <li><a href="#collision-handling">Collision Handling</a></li>
  <li><a href="#physics-model">Physics Model</a></li>
  <li><a href="#design-notes">Design Notes</a></li>
  <li><a href="#demonstration">Visual Demos</a></li>
  <li><a href="#build-instructions-linux">Build Instructions</a></li>
  <li><a href="#dependencies">Dependencies</a></li>
</ul>

<hr>

<h2 id="collision-handling">Collision Handling</h2>

<h3 id="convex-shapes">Convex Shapes</h3>

<p>Collision detection between convex polygons uses the <strong>Separating Axis Theorem (SAT)</strong>. Contact points are computed using face clipping and penetration depth is used to generate response impulses.</p>

<h3 id="circle-polygon-interactions">Circle–Polygon Interactions</h3>

<p>Circles are handled with a <strong>face projection method</strong>, allowing smooth resolution against polygon edges without excessive branching or shape-specific logic.</p>

<h3 id="wall-and-border-contacts">Wall and Border Contacts</h3>

<p>Simple deepest-point resolution is used to prevent tunneling and maintain expected object boundaries in confined scenes.</p>

<hr>

<h2 id="physics-model">Physics Model</h2>

<h3 id="impulse-resolution">Impulse Resolution</h3>

<p>Collisions are resolved using a basic impulse solver. Linear and angular velocities are updated using the relative velocity at the contact point and the combined inverse mass and inertia of the two bodies.</p>

<p><strong>Impulse formula:</strong></p>

$$
J =
\frac{
-(1 + e) \cdot (\vec{v}_{rel} \cdot \vec{n})
}{
\frac{1}{m_A} + \frac{1}{m_B} +
\frac{(\vec{r}_A \times \vec{n})^2}{I_A} +
\frac{(\vec{r}_B \times \vec{n})^2}{I_B}
}
$$

<p><strong>Where:</strong></p>

<ul>
  <li>$e$ is the restitution coefficient</li>
  <li>$\vec{v}_{rel}$ is the relative velocity at the contact</li>
  <li>$\vec{n}$ is the contact normal</li>
  <li>$m$, $I$ are the mass and moment of inertia</li>
</ul>

<h3 id="friction">Friction</h3>

<p>Basic Coulomb friction is supported. Tangential impulses are clamped relative to the normal impulse, allowing simple sliding and resting contact behavior.</p>

<hr>

<h2 id="design-notes">Design Notes</h2>

<ul>
  <li>Uses <strong>Structure of Arrays (SoA)</strong> for better cache behavior during updates.</li>
  <li>Simulation runs on a <strong>fixed timestep</strong> for consistency and repeatability.</li>
  <li>Engine <strong>state</strong> is stored in a <strong>self-contained context</strong>, allowing integration into other applications or environments.</li>
  <li>The API is exposed via the interface header: <a href="src/engine/interface.h"><code>src/engine/interface.h</code></a>.</li>
  <li>Usage examples are provided in the <a href="examples/"><code>examples</code></a> directory.</li>
</ul>

<hr>

<h2 id="demonstration">Demonstration</h2>

<h3 id="convex-circle-impulse-and-friction-resolution">Convex + Circle Impulse and Friction Resolution</h3>

<p>Demonstrates contact resolution, restitution, and friction between convex shapes and circles.</p>

<p><img src="media/convex_circle_impulse.gif" alt="Convex + circle impulse and friction resolution"></p>

<p><a href="examples/collision_demo.cpp"><code>View source code</code></a></p>

<hr>

<h3 id="mass-disparity-stability-test">Mass Disparity Stability Test</h3>

<p>Tests stability under differences in body mass and geometry.</p>

<p><img src="media/50convex_50circle.gif" alt="Piling stability under mass disparity"></p>

<hr>

<h2 id="build-instructions-linux">Build Instructions (Linux)</h2>

<pre><code class="language-bash">sudo pacman -S git clang cmake make sfml glm
git clone https://github.com/cianleypoldt/SAT-Impulse-Physics.git
mkdir Zenoa/build; cd Zenoa/build
cmake ..; make
./Zenoa
</code></pre>

<hr>

<h2 id="dependencies">Dependencies</h2>

<ul>
  <li><strong>GLM</strong> – Vector math and linear algebra</li>
  <li><strong>SFML</strong> – Debug rendering and input</li>
</ul>

<hr>

<h2 id="license">License</h2>

<p>Zenoa is released under the MIT License.</p>
