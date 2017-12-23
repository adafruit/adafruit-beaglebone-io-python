# Generating API documentation 

This folder contains the required files to automatically generate the Adafruit Beaglebone I/O Python API documentation, partly from the code docstrings and partly from files in reStructuredText format.

```
├── conf.py    <- Sphinx configuration file
├── index.rst  <- Documentation will be generated based on the master index
└── Makefile   <- Auxiliary Makefile to build documentation
```

The tools used are [Sphinx](http://www.sphinx-doc.org) to extract the documentation and publish it in HTML format for online viewing, in combination with [Readthedocs](http://readthedocs.io). Readthedocs automatically executes Sphinx via webhooks triggered by Github commits, and publishes the resulting docs for all tracked branches or tags. Generally Readthedocs will be set up to track stable release tags and the master branch.

## Building the documentation

The documentation can also be built on a local checkout of the project:

First ensure you've got sphinx installed:

```
sudo pip install sphinx
```

Then you can build the HTML docs:

```
cd docs
make html
```

Once Sphinx has built the documentation, you can open the main index file with your browser: `_build/html/index.html`

Notes:

- The build process will create three additional temporary directories: `_build`, `_static` and `_templates` that will not be version-controlled. You can use `make clean` to remove their contents if you wish to do so.
- The html theme from files built locally is different from the online readthedocs theme. See the `docs/config.py` `html_theme` variable. The main reason is not to introduce another dependency to install the readthedocs theme, but as a side effect, it also helps visually distinguishing the locally-built documentation from the online version.

## Readthedocs maintenance

At every release that includes documenation (most probably 1.0.10 will be the first one), the release's branch or tag needs to be selected in the web UI and marked as active.

After this, documentation will automatically be generated and published for that release. It will be available at the same URL as the main documentation, and a link with the version number will be shown, where it can be accessed from.

Optionally, the 'stable' URL slug can be pointed to that release branch. Otherwise, the 'stable' slug can also be deactivated for less maintenance overhead.

The 'latest' URL slug will always be pointing at the repo's master branch.

## Notes

Ideally, all API documentation would be written in the source files as Python docstrings, and sphinx would simply extract it. This is actually the case with the `Encoder` module, which is pure Python.

However, most of the code is written as C extensions. While they do provide docstrings once they are built, Sphinx does not natively support extracting them. There is [a workaround](https://stackoverflow.com/a/30110104/9022675) to do this, but it involves first building the extensions, installing them and hardcoding a path. While it might work for locally-built documentation, it's unlikely that readthedocs support this option.

For the sake of keeping things simple and with less maintenance, the approach of documenting the C-generated API in separate `.rst` files (one for each Python module) has been taken.

This has the advantage of having a definition of the API in one place, but it also poses the disadvantage of some duplication, as the C modules do define some docstrings for their objects. Then again, the API itself has hardly changed in the last few years, and the Beaglebone is a mature platform, so it's unlikely that this will add a significant maintenance overhead.

- The documentation in the `.rst` files is written in [reStructuredText](http://docutils.sourceforge.net/rst.html), extended with Sphinx markup for defining the objects.
- The documentation in Python modules follows the Google readable docstring markup, which also builds upon reStructuredText and is fully supported by Sphinx.

## Further reference

- [Google readable docstring markup](https://google.github.io/styleguide/pyguide.html?showone=Comments#Comments)
- [Google docstring examples](http://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_google.html)
- [More Google docstring examples](http://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_google.html)
- [Sphinx docstring markup](http://www.sphinx-doc.org/en/stable/domains.html#the-python-domain)
- [reStructuredText primer](http://www.sphinx-doc.org/en/stable/rest.html#rst-primer)


