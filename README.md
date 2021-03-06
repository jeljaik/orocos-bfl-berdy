# orocos-bfl-berdy
Orocos Bayesian Filtering Library Fork

This library is based on Orocos BFL 0.8.0 and will be extended and used for implementing BERDY's framework. 

# Original README
$Id: README 28203 2007-05-29 07:58:40Z tdelaet $
// 
// BFL: BAYESIAN FILTERING LIBRARY
// 
// 
// Copyright (C) 2002/2003/2004 Klaas Gadeyne <first dot last at gmail dot com>
//  
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  

This library encoporates ideas from several available software
libraries:

- Scene (Andrew Davison).  See
<http://www.robots.ox.ac.uk/~ajd/Scene/>

- Bayes++ (from ACFR). See
<http://www.acfr.usyd.edu.au/technology/bayesianfilter/Bayes++.htm> 

- The CES programming library (Sebastian Thrun).  See 
<http://www-2.cs.cmu.edu/afs/cs.cmu.edu/user/thrun/public_html/papers/thrun.ces-tr.html>

- Our own research with Bayesian methods for compliant motion problems
<http://www.mech.kuleuven.be/pma/research/manip/default_en.phtml>

It's most important features are:
- Released under the GNU LGPL licence
- Wrapper around matrix and RNG libraries, so you can use your own
  favourite matrix library.
  At 2004/03/02 wrappers exist for
    * The matrix/RNG wrapper library of LTIlib
  <http://ltilib.sourceforge.net/doc/homepage/index.shtml>: a library
  with algorithms and data structures frequently used in image
  processing and computer vision.
    * NEWMAT <http://www.robertnz.net/nm_intro.htm> Matrix Library
    * boost <http://www.boost.org/> RNG


- "Bayesian unifying Design".  This allows to incorporate any Bayesian
  filtering algorithm!

  Currently the following filter schemes are implemented.
  * Standard KF, EKF, IEKF and Non-minimal State KF (See
  <http://people.mech.kuleuven.ac.be/~tlefebvr/publicaties/BayesStat.ps.gz> 

  * Standard Particle filter (arbitrary proposal), BootstrapFilter
  (Proposal = System Model PDF), Auxiliary Particle filter, Extended
  Kalman Particle Filter. 

For further details about the design ideas, see the poster about the
library presented at Valencia 7, a conference about Bayesian
Statistics, available from
<http://people.mech.kuleuven.ac.be/~kgadeyne/doctoraat.html>
Also have a look at the filtering libraries home page
<http://www.orocos.org/bfl>

Tinne De Laet Contributed a tutorial which can be found on the
website.
<http://people.mech.kuleuven.be/~tdelaet/bfl_doc/getting_started_guide/getting_started_guide.html>
It discusses how to construct your first filter in bfl. 

Wim Meeussen and Tinne De Laet contributed a installation guide which can be
found on the website.
<http://people.mech.kuleuven.be/~tdelaet/bfl_doc/installation_guide/installation_guide.html>
