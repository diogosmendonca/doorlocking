import React, { Component } from "react";
import {Link} from 'react-router-dom'

class Menu extends Component {


    componentDidMount(){
        this.layout   = document.getElementById('layout');
        this.menu     = document.getElementById('menu');
        this.menuLink = document.getElementById('menuLink');
        this.content  = document.getElementById('main');
    }

    handleHideMenu(e){
        if (this.menu.className.indexOf('active') !== -1) {
            this.toggleAll(e);
        }
    }
    
    toggleClass(element, className) {
        var classes = element.className.split(/\s+/),
            length = classes.length,
            i = 0;

        for(; i < length; i++) {
          if (classes[i] === className) {
            classes.splice(i, 1);
            break;
          }
        }
        // The className is not found
        if (length === classes.length) {
            classes.push(className);
        }

        element.className = classes.join(' ');
    }

    toggleAll(e) {
        var active = 'active';

        //e.preventDefault();
        this.toggleClass(this.layout, active);
        this.toggleClass(this.menu, active);
        this.toggleClass(this.menuLink, active);
    }

    render(){
        return(
            <>
                <a href="#menu" id="menuLink" className="menu-link" onClick={(e) => this.toggleAll(e)}>
                    <span></span>
                </a>

                <div id="menu">
                    <div className="pure-menu">
                        <a className="pure-menu-heading" href="/">DoorLocking.App</a>
                        <ul className="pure-menu-list">
                            <li className="pure-menu-item"><Link onClick={(e) => this.handleHideMenu(e)} to='/open_door' className="pure-menu-link">Open Door</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.handleHideMenu(e)} to='/register_user' className="pure-menu-link">New User</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.handleHideMenu(e)} to='/list_users' className="pure-menu-link">List/Change Users</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.handleHideMenu(e)} to='/config' className="pure-menu-link">Configuration</Link></li>
                            <li className="pure-menu-item"><Link onClick={(e) => this.handleHideMenu(e)} to='/logout_handler' className="pure-menu-link">Logout</Link></li>
                        </ul>
                    </div>
                </div>
            </>
        );
    }
}

export default Menu;