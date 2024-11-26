![image](https://github.com/user-attachments/assets/e978e302-5f31-4814-9d2c-6e7ee3e651b4)


<h1> Intro </h1>
  <p>Picoma os is a simple 32 bit console-based(at least for now) OS. It works only in virtual enviroment and wasn't tested on real hardware. It's still under construction, so it's unstable. Current version is 0.0.2. End goal of this OS is to have simple graphical user-friendly interface along with very simplistic user applications, like on very old commercial OS'es</p>
  <br>
  
<h2>How to use it</h2>

```
1. git clone https://github.com/JabloZ/picoma_os.git
2. cd picoma_os
3 ./setup_gcc_cp.sh
4. make clean && make && make run
```
<br>
<h1>What does it offer</h1>

<h3>Opofs filesystem</h3>

<p>Very simple LBA based filesystem. For now it only supports 1.44Mb floppies</p>
<br>
COMMANDS: 
<ul>
  <li><strong>help </strong> - shows list of available commands</li>
  <li><strong>la [path] </strong> - shows files and directories for path</li>
  <li><strong>pf [path to file] </strong> - prints out file contents</li>
  <li><strong>pte [path to file] </strong> - opens file in simple text editor.</li>
    <ul>
      <li><strong>ESCAPE</strong> - switch between command and insert mode.</li>
      <li><strong>qs</strong> - quit and save.</li>
      <li><strong>q</strong> - quit and <strong>don't</strong> save.</li>
    </ul>
  <li><strong>cd [path]</strong> - change directory, [..] to go back </li>
  <li><strong>mkf [path] [filename]</strong> - creates file </strong></li>
  <li><strong>mkd [path] [directory name]</strong> - creates directory </strong></li>
  <li><strong>rmv [path]</strong> - deletes directory/file </strong></li>
</ul>

![image](https://github.com/user-attachments/assets/8634f9dd-a97a-4e51-892d-2a5a69da4a49)

<br>
<h3>Simple memory management</h3>
<p>This OS has it's own Physical and Virtual memory manager. It also supports dynamic memory allocation with buddy tree-like allocator.</p>

![image](https://github.com/user-attachments/assets/de96e3fe-3603-4733-8932-fe78589aedab)

<br>
<h3>Text editor</h3>
<p><strong>PTE</strong> is a simple text editor that supports editing files freely. click ESCAPE and type q to quit, qs to save and quit. to go back to insert mode, click ESCAPE again.</p>

![image](https://github.com/user-attachments/assets/ad5567fe-43d3-4421-b4a5-4698b586090f)

<br>
<h3>More</h3>
<p>There are more features, but they are related to functioning of the system, not user experience (IRQ, PIC, FDC, GDT, IDT and c basic libraries) so i didn't mention them. There's still a lot to do, and this project is very time consuming so it won't be changing fast.</p>

<br>
<h2>Credits</h2>
<ul>
<li>
<a href="https://wiki.osdev.org">OSDev wiki</a>  
</li>
<li>
<a href="https://www.youtube.com/@nanobyte-dev">Nanobyte</a>  
</li>


</ul>
