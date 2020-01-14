import React, { Component } from "react";
import {Link} from 'react-router-dom'

class Menu extends Component {

    render(){
        return(
            <>
                <a href="#menu" id="menuLink" className="menu-link">
                    <span></span>
                </a>

                <div id="menu">
                    <div className="pure-menu">
                        <a className="pure-menu-heading" href="/">DoorLocking.App</a>
                        <ul className="pure-menu-list">
                            <li className="pure-menu-item"><Link to='/open_door' className="pure-menu-link">Open Door</Link></li>
                            <li className="pure-menu-item"><Link to='/register_user' className="pure-menu-link">New User</Link></li>
                            <li className="pure-menu-item"><Link to='/list_users' className="pure-menu-link">List/Change Users</Link></li>
                            <li className="pure-menu-item"><Link to='/config' className="pure-menu-link">Configuration</Link></li>
                            <li className="pure-menu-item"><Link to='/logout_handler' className="pure-menu-link">Logout</Link></li>
                        </ul>
                    </div>
                </div>
            </>
        );
    }
}

export default Menu;